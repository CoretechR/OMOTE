using System.Text;
using System.Xml;

namespace LvglSubmoduleProjectFileGenerator
{

    internal class Program
    {
        private static List<string> FilterNames =
            new List<string>();
        private static List<(string Target, string Filter)> HeaderNames =
            new List<(string Target, string Filter)>();
        private static List<(string Target, string Filter)> SourceNames =
            new List<(string Target, string Filter)>();
        private static List<(string Target, string Filter)> OtherNames =
            new List<(string Target, string Filter)>();

        private static string[] HeaderFileTypes = new string[]
        {
            ".h",
            ".hh",
            ".hpp",
            ".hxx",
            ".h++",
            ".hm",
            ".inl",
            ".inc",
            ".ipp"
        };

        private static string[] SourceFileTypes = new string[]
        {
            ".cpp",
            ".c",
            ".cc",
            ".cxx",
            ".c++",
            ".cppm",
            ".ixx"
        };

        private static string[] ForceInOthersList = new string[]
        {
            @".github",
            @"docs",
            @"tests",
            @"lvgl\env_support",
            @"lvgl\scripts",
            @"freetype\"
        };

        private static void EnumerateFolder(
            string Path,
            bool ForceInOthers = false)
        {
            DirectoryInfo Folder = new DirectoryInfo(Path);

            FilterNames.Add(Folder.FullName);

            foreach (var Item in Folder.GetDirectories())
            {
                bool CurrentForceInOthers = false;
                foreach (var ListItem in ForceInOthersList)
                {
                    if (Item.FullName.Contains(ListItem))
                    {
                        CurrentForceInOthers = true;
                        break;
                    }
                }
                EnumerateFolder(
                    Item.FullName,
                    ForceInOthers || CurrentForceInOthers);
            }

            foreach (var Item in Folder.GetFiles())
            {
                (string Target, string Filter) Current =
                    (Item.FullName, Item.Directory.FullName);

                if (ForceInOthers)
                {
                    OtherNames.Add(Current);
                    continue;
                }

                if (HeaderFileTypes.Contains(Item.Extension))
                {
                    HeaderNames.Add(Current);
                }
                else if (SourceFileTypes.Contains(Item.Extension))
                {
                    SourceNames.Add(Current);
                }
                else
                {
                    OtherNames.Add(Current);
                }
            }
        }


        static void Main(string[] args)
        {
            string Root = GitRepositoryUtilities.GetRepositoryRoot();

            Console.WriteLine(Root);

            string RootPath = Path.GetFullPath(Root + @"\LvglPlatform\");

            EnumerateFolder(RootPath + @"freetype");
            EnumerateFolder(RootPath + @"lvgl");
            EnumerateFolder(RootPath + @"lv_drivers");
            OtherNames.Add((
                "$(MSBuildThisFileDirectory)LvglPlatform.props",
                string.Empty));

            List<(string, string)> NewHeaderNames = new List<(string, string)>();
            List<(string, string)> NewSourceNames = new List<(string, string)>();
            List<(string, string)> NewOtherNames = new List<(string, string)>();

            foreach (var HeaderName in HeaderNames)
            {
                NewHeaderNames.Add((
                    HeaderName.Item1.Replace(
                        RootPath,
                        "$(MSBuildThisFileDirectory)"),
                    HeaderName.Item2));
            }

            foreach (var SourceName in SourceNames)
            {
                NewSourceNames.Add((
                    SourceName.Item1.Replace(
                        RootPath,
                        "$(MSBuildThisFileDirectory)"),
                    SourceName.Item2));
            }

            foreach (var OtherName in OtherNames)
            {
                NewOtherNames.Add((
                    OtherName.Item1.Replace(
                        RootPath,
                        "$(MSBuildThisFileDirectory)"),
                    OtherName.Item2));
            }


            XmlWriterSettings WriterSettings = new XmlWriterSettings();
            WriterSettings.Indent = true;
            WriterSettings.IndentChars = "  ";
            WriterSettings.NewLineChars = "\r\n";
            WriterSettings.NewLineHandling = NewLineHandling.Replace;
            WriterSettings.Encoding = new UTF8Encoding(true);

            {
                XmlDocument Document =
                    VisualStudioProjectUtilities.CreateCppSharedProject(
                        Guid.NewGuid(),
                        NewHeaderNames,
                        NewSourceNames,
                        NewOtherNames);
                Document.InnerXml = Document.InnerXml.Replace(
                    RootPath,
                    "");
                XmlWriter Writer = XmlWriter.Create(
                    string.Format(
                        @"{0}\LvglPlatform.vcxitems",
                        RootPath),
                    WriterSettings);
                Document.Save(Writer);
                Writer.Flush();
                Writer.Dispose();
            }

            {
                XmlDocument Document =
                    VisualStudioProjectUtilities.CreateCppSharedFilters(
                        FilterNames,
                        NewHeaderNames,
                        NewSourceNames,
                        NewOtherNames);
                Document.InnerXml = Document.InnerXml.Replace(
                    RootPath,
                    "");
                XmlWriter Writer = XmlWriter.Create(
                    string.Format(
                        @"{0}\LvglPlatform.vcxitems.filters",
                        RootPath),
                    WriterSettings);
                Document.Save(Writer);
                Writer.Flush();
                Writer.Dispose();
            }

            Console.WriteLine("Hello, World!");

            Console.ReadKey();
        }
    }
}

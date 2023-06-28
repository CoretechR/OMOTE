using System.Xml;

namespace LvglSubmoduleProjectFileGenerator
{
    public class VisualStudioProjectUtilities
    {
        private static readonly string DefaultNamespaceString =
            @"http://schemas.microsoft.com/developer/msbuild/2003";

        private static XmlElement AppendTagToElement(
            XmlElement Element,
            string Name)
        {
            XmlElement Tag = Element.OwnerDocument.CreateElement(
                Name,
                DefaultNamespaceString);
            Element.AppendChild(Tag);
            return Tag;
        }

        private static XmlElement AppendPropertyGroupToProject(
            XmlElement Project)
        {
            return AppendTagToElement(Project, "PropertyGroup");
        }

        private static XmlElement AppendPropertyGroupToProject(
            XmlElement Project,
            string Label)
        {
            XmlElement Element = AppendPropertyGroupToProject(Project);
            Element.SetAttribute("Label", Label);
            return Element;
        }

        private static XmlElement AppendItemGroupToProject(
            XmlElement Project)
        {
            return AppendTagToElement(Project, "ItemGroup");
        }

        private static XmlElement AppendItemToItemGroup(
            XmlElement ItemGroup,
            string Type,
            string Target)
        {
            XmlElement Element = AppendTagToElement(ItemGroup, Type);
            Element.SetAttribute("Include", Target);
            return Element;
        }

        private static void AppendTagToElement(
            XmlElement Element,
            string Name,
            string Content)
        {
            XmlElement Tag = AppendTagToElement(Element, Name);
            Tag.InnerText = Content;
        }

        public static void AppendItemsToCppProject(
            XmlElement Project,
            List<(string Target, string Filter)> HeaderNames,
            List<(string Target, string Filter)> SourceNames,
            List<(string Target, string Filter)> OtherNames)
        {
            XmlElement HeaderItems = AppendItemGroupToProject(Project);
            foreach (var Name in HeaderNames)
            {
                AppendItemToItemGroup(HeaderItems, "ClInclude", Name.Target);
            }

            XmlElement SourceItems = AppendItemGroupToProject(Project);
            foreach (var Name in SourceNames)
            {
                AppendItemToItemGroup(SourceItems, "ClCompile", Name.Target);
            }

            XmlElement OtherItems = AppendItemGroupToProject(Project);
            foreach (var Name in OtherNames)
            {
                AppendItemToItemGroup(OtherItems, "None", Name.Target);
            }
        }

        public static void AppendItemsToCppFilters(
            XmlElement Project,
            List<string> FilterNames,
            List<(string Target, string Filter)> HeaderNames,
            List<(string Target, string Filter)> SourceNames,
            List<(string Target, string Filter)> OtherNames)
        {
            XmlElement FilterItems = AppendItemGroupToProject(Project);
            foreach (var FilterName in FilterNames)
            {
                XmlElement FilterItem = AppendItemToItemGroup(
                    FilterItems,
                    "Filter",
                    FilterName);
                AppendTagToElement(
                    FilterItem,
                    "UniqueIdentifier",
                    string.Format("{{{0}}}", Guid.NewGuid()));
            }

            XmlElement HeaderItems = AppendItemGroupToProject(Project);
            foreach (var Name in HeaderNames)
            {
                XmlElement Item = AppendItemToItemGroup(
                    HeaderItems,
                    "ClInclude",
                    Name.Target);
                AppendTagToElement(Item, "Filter", Name.Filter);
            }

            XmlElement SourceItems = AppendItemGroupToProject(Project);
            foreach (var Name in SourceNames)
            {
                XmlElement Item = AppendItemToItemGroup(
                    SourceItems,
                    "ClCompile",
                    Name.Target);
                AppendTagToElement(Item, "Filter", Name.Filter);
            }

            XmlElement OtherItems = AppendItemGroupToProject(Project);
            foreach (var Name in OtherNames)
            {
                XmlElement Item = AppendItemToItemGroup(
                    OtherItems,
                    "None",
                    Name.Target);
                AppendTagToElement(Item, "Filter", Name.Filter);
            }
        }

        public static XmlDocument CreateCppSharedProject(
            Guid ProjectGuid,
            List<(string Target, string Filter)> HeaderNames,
            List<(string Target, string Filter)> SourceNames,
            List<(string Target, string Filter)> OtherNames)
        {
            XmlDocument Document = new XmlDocument();

            Document.InsertBefore(
                Document.CreateXmlDeclaration("1.0", "utf-8", null),
                Document.DocumentElement);

            XmlElement Project = Document.CreateElement(
                "Project",
                DefaultNamespaceString);
            Project.SetAttribute(
                "ToolsVersion",
                "4.0");

            XmlElement GlobalPropertyGroup = AppendPropertyGroupToProject(
                Project,
                "Globals");    
            AppendTagToElement(
                GlobalPropertyGroup,
                "ItemsProjectGuid",
                string.Format("{{{0}}}", ProjectGuid));

            AppendItemsToCppProject(
                Project,
                HeaderNames,
                SourceNames,
                OtherNames);

            Document.AppendChild(Project);

            return Document;
        }

        public static XmlDocument CreateCppSharedFilters(
            List<string> FilterNames,
            List<(string Target, string Filter)> HeaderNames,
            List<(string Target, string Filter)> SourceNames,
            List<(string Target, string Filter)> OtherNames)
        {
            XmlDocument Document = new XmlDocument();

            Document.InsertBefore(
                Document.CreateXmlDeclaration("1.0", "utf-8", null),
                Document.DocumentElement);

            XmlElement Project = Document.CreateElement(
                "Project",
                DefaultNamespaceString);
            Project.SetAttribute(
                "ToolsVersion",
                "4.0");

            AppendItemsToCppFilters(
                Project,
                FilterNames,
                HeaderNames,
                SourceNames,
                OtherNames);

            Document.AppendChild(Project);

            return Document;
        }
    }
}

# ARM32 Support Removed Notice

For deliberated consideration, This project will remove the ARM32 support on
the Windows platform. Here are the reasons:

- The latest version of ARM32 version for Windows desktop is Redstone 2 Insider
  Build 15035. I know Windows RT 8.1 and Windows 10 IoT Core aren't in the 
  stage of end of support, but most of daily users are drop their devices 
  (Windows RT 8.x tablets) or have a better solution (Windows 10 IoT Core users
  on Raspberry Pi devices should migrate to Linux or ARM64 version for Windows 
  10 desktop).
- Future ARM processors are deprecating ARM32 ISA support, and Apple Silicon M1
  had dropped the ARM32 support at all. So we can't run ARM32 version of Windows
  desktop applications on these devices. According to
  https://learn.microsoft.com/en-us/windows/arm/arm32-to-arm64, we will know
  Microsoft will remove the support for 32-bit ARM versions of applications in
  a future release of Windows 11.
- Reduce the size of release package and make the continuous integration faster.

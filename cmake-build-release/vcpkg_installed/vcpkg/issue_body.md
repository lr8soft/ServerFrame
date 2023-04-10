Package: boost-static-assert[core]:x64-windows -> 1.81.0#2

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.35.32216.1
-    vcpkg-tool version: 2023-03-29-664f8bb619b752430368d0f30a8289b761f5caba
    vcpkg-scripts version: 7f59e0013 2023-04-06 (3 days ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached boostorg-static_assert-boost-1.81.0.tar.gz.
-- Extracting source C:/Users/lrsoft/.clion-vcpkg/vcpkg/downloads/boostorg-static_assert-boost-1.81.0.tar.gz
CMake Error at scripts/cmake/vcpkg_extract_source_archive.cmake:159 (file):
  file RENAME failed to rename

    C:/Users/lrsoft/.clion-vcpkg/vcpkg/buildtrees/boost-static-assert/src/ost-1.81.0-4234361f3e.clean.tmp/static_assert-boost-1.81.0

  to

    C:/Users/lrsoft/.clion-vcpkg/vcpkg/buildtrees/boost-static-assert/src/ost-1.81.0-4234361f3e.clean

  because: �ܾ����ʡ�

  

Call Stack (most recent call first):
  scripts/cmake/vcpkg_extract_source_archive_ex.cmake:8 (vcpkg_extract_source_archive)
  scripts/cmake/vcpkg_from_github.cmake:113 (vcpkg_extract_source_archive_ex)
  buildtrees/versioning_/versions/boost-static-assert/bf5777cf607209edba1236a1cf87721ad896ead5/portfile.cmake:3 (vcpkg_from_github)
  scripts/ports.cmake:147 (include)



```


**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "serverframe",
  "version-string": "1.0.0",
  "builtin-baseline": "7f59e0013648f0dd80377330b770b414032233cb",
  "dependencies": [
    {
      "name": "curl",
      "version>=": "8.0.1"
    },
    {
      "name": "jsoncpp",
      "version>=": "1.9.5"
    },
    {
      "name": "boost",
      "version>=": "1.81.0#3"
    }
  ]
}

```
</details>

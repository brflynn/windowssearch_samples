# windowssearch_samples
Here you'll find Win32/C# samples for APIs that interact with the Windows Search Service and specific projects highlighting different scenarios for developers

# Identify Current Indexing Scopes
listscopes - A simple tool that lists the current scopes registered on the system. 

# Excluding Folders from the Index
A lot of times what makes the most sense for applications running as the current user is to stick configuration and settings files in the %USERPROFILE% directory. By default this
is an indexed location meaning the indexing service will crawl the content and actually open the files including them in the system index. If you don't want your applications data in the system index or if it is interfering with your application excluding it is an option.

An exception to the above:
  - By default the %USERPROFILE%\AppData folder is excluded, this is a safe place to store files.
  - The pattern %USERPROFILE%\.*\* is automatically excluded from the index as well.



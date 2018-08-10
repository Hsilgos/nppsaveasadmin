import sys

version_str = sys.argv[1]
version_array = version_str.split('.')
if (len(version_array) < 4):
  for i in range(len(version_array), 4):
    version_array.append('0')

del version_array[4:]

printable_version = '#define SAVE_AS_ADMIN_VERSION "' + version_str + '"'
printable_version_unicode = '#define SAVE_AS_ADMIN_VERSION_UNICODE L"' + version_str + '"'
resource_version = '#define SAVE_AS_ADMIN_VERSION_DIGITAL '  + ', '.join(version_array)

print(printable_version)
print(printable_version_unicode)
print(resource_version)
print('')

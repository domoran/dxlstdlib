getReg.exe will print out the value of an registry entry. It can be used for example to find the path to a windows program, or check if a program is installed from the command line. It is used the following way:

getReg.exe <base> <key path> <value>

where:
    <base>     is one of:
                    HKCR -> the key will be searched in HKEY_CLASSES_ROOT
                    HKCU -> the key will be searched in HKEY_CURRENT_USER
                    HKLM -> the key will be searched in HKEY_LOCAL_MACHINE
                    HKU  -> the key will be searched in HKEY_USERS
                    HKCC -> the key will be searched in HKEY_CURRENT_CONFIG        
    <key path> is the path to the registry key    
    <value>    the name of the value to print 
    
Example: 
    getReg.exe HKCU SOFTWARE\Microsoft\Office\11.0\Word\InstallRoot Path
 
--> returns the path where microsoft Office 2003 is installed
    
    
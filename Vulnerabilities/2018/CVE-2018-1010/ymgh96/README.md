# Detecting-the-patch-of-CVE-2018-1010<br>
This C++ code developed on Visual Studio 2017. This program works with cmd's commands.<br>
In this program first, we check your operating system version and then according to Microsoft's notification checks whether you have installed the patch for CVE-2018-1010 vulnerability with your Windows or not.<br>
you can test this code with the following commands in windows:<br>
1) checking OS version:<br>
systeminfo | findstr /B /C:"OS Name" /C:"OS Version"<br>
2) checking the installation of patch:<br>
wmic qfe | find "the_number_of_desired_patch"<br>

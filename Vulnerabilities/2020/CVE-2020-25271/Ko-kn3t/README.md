# CVE-2020-25271

# PHPGurukul hospital-management-system-in-php 4.0 allows XSS via
> admin/patient-search.php,

> doctor/search.php,

> book-appointment.php,

> doctor/appointment-history.php, or

> admin/appointment-history.php.


#Vendor - PHPGurukul

#Product -https://phpgurukul.com/hospital-management-system-in-php  V 4.0

#Vulnerability Type - Cross Site Scripting (XSS)

#Addition Information - Single XSS payload will trigger in all Dashboard, so account take over will be occurred.

#Affected Component - Books > New Book ,[ http:///lms/index.php?page=books] http:///lms/index.php?page=books

#Attack Type- Local

#Privilege Escalation - true

#Impact Code execution - true

> ***Attack Vector***
> --------------------
>
>
> Cross site scripting in  Admin | View Patients (http://localhost/hospital/hms/admin/patient-search.php)
>
>
> Stored XSS in User | Dashboard ( Name field)
>
>
> Cross site scripting in  Doctor | Manage Patients (http://localhost/hospital/hms/doctor/search.php)
>
>
> Install Hospital Management System V 4.0
>
>
>***1) Patient Module***
> ________________
>
>  i.  Create patient account account with username "<script>alert(`XSS`);</script>" , XSS will be triggered in every page of Patient Dashboard
> 
>  ii. Make an appointment  at "Book Appointment" (http://localhost/hospital/hms/book-appointment.php).
>
>
>
> ***2) Doctor Module***
> ________________
>
>  i. Login as doctor who was requested appointment by malicious patient
> 
>  ii. Go to "Appointment History" (http://localhost/hospital/hms/doctor/appointment-history.php), XSS will trigger also.
> 
> 
>
> ***3) Admin Module***
>
>  i.  Login as admin
>  ii. Go to "Appointment History" (http://localhost/hospital/hms/admin/appointment-history.php), XSS will trigger.

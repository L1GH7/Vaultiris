<!doctype html>
<html>
	<head>
		<title>CVE-2021-34429</title>
		<%!
			String title = "Developed by ColdFusionX";
		%>
	</head>
	<body>
		<h1 style="text-align:center"><%= title %></h1>
		<p style="text-align:center">
			Just for CVE testing purpose
		</p>
		<p style="text-align:center">
			<%= new java.util.Date() %>
		</p>
		<p style="text-align:center">
			You are from <%= request.getRemoteAddr() %>
		</p>
		
	</body>
</html>


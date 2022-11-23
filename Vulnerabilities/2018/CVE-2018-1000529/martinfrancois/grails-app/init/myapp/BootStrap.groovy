package myapp

class BootStrap {

    def init = { servletContext ->
        Test test = new Test(name: "Test1<script>alert('XSS');</script>").save()
    }
    def destroy = {
    }
}

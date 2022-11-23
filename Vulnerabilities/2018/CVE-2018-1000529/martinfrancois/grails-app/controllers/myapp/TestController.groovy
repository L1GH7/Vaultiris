package myapp

import grails.validation.ValidationException
import static org.springframework.http.HttpStatus.*

class TestController {

    TestService testService

    static allowedMethods = [save: "POST", update: "PUT", delete: "DELETE"]

    def index(Integer max) {
        params.max = Math.min(max ?: 10, 100)
        respond testService.list(params), model:[testCount: testService.count()]
    }

    def show(Long id) {
        respond testService.get(id)
    }

    def create() {
        respond new Test(params)
    }

    def save(Test test) {
        if (test == null) {
            notFound()
            return
        }

        try {
            testService.save(test)
        } catch (ValidationException e) {
            respond test.errors, view:'create'
            return
        }

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message', args: [message(code: 'test.label', default: 'Test'), test.id])
                redirect test
            }
            '*' { respond test, [status: CREATED] }
        }
    }

    def edit(Long id) {
        respond testService.get(id)
    }

    def update(Test test) {
        if (test == null) {
            notFound()
            return
        }

        try {
            testService.save(test)
        } catch (ValidationException e) {
            respond test.errors, view:'edit'
            return
        }

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.updated.message', args: [message(code: 'test.label', default: 'Test'), test.id])
                redirect test
            }
            '*'{ respond test, [status: OK] }
        }
    }

    def delete(Long id) {
        if (id == null) {
            notFound()
            return
        }

        testService.delete(id)

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.deleted.message', args: [message(code: 'test.label', default: 'Test'), id])
                redirect action:"index", method:"GET"
            }
            '*'{ render status: NO_CONTENT }
        }
    }

    protected void notFound() {
        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.not.found.message', args: [message(code: 'test.label', default: 'Test'), params.id])
                redirect action: "index", method: "GET"
            }
            '*'{ render status: NOT_FOUND }
        }
    }
}

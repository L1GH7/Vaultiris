package myapp

import grails.testing.mixin.integration.Integration
import grails.gorm.transactions.Rollback
import spock.lang.Specification
import org.hibernate.SessionFactory

@Integration
@Rollback
class TestServiceSpec extends Specification {

    TestService testService
    SessionFactory sessionFactory

    private Long setupData() {
        // TODO: Populate valid domain instances and return a valid ID
        //new Test(...).save(flush: true, failOnError: true)
        //new Test(...).save(flush: true, failOnError: true)
        //Test test = new Test(...).save(flush: true, failOnError: true)
        //new Test(...).save(flush: true, failOnError: true)
        //new Test(...).save(flush: true, failOnError: true)
        assert false, "TODO: Provide a setupData() implementation for this generated test suite"
        //test.id
    }

    void "test get"() {
        setupData()

        expect:
        testService.get(1) != null
    }

    void "test list"() {
        setupData()

        when:
        List<Test> testList = testService.list(max: 2, offset: 2)

        then:
        testList.size() == 2
        assert false, "TODO: Verify the correct instances are returned"
    }

    void "test count"() {
        setupData()

        expect:
        testService.count() == 5
    }

    void "test delete"() {
        Long testId = setupData()

        expect:
        testService.count() == 5

        when:
        testService.delete(testId)
        sessionFactory.currentSession.flush()

        then:
        testService.count() == 4
    }

    void "test save"() {
        when:
        assert false, "TODO: Provide a valid instance to save"
        Test test = new Test()
        testService.save(test)

        then:
        test.id != null
    }
}

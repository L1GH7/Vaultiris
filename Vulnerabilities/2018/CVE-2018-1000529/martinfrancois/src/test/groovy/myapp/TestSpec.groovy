package myapp

import grails.testing.gorm.DomainUnitTest
import spock.lang.Specification

class TestSpec extends Specification implements DomainUnitTest<Test> {

    def setup() {
    }

    def cleanup() {
    }

    void "test something"() {
        expect:"fix me"
            true == false
    }
}

package myapp

import grails.gorm.services.Service

@Service(Test)
interface TestService {

    Test get(Serializable id)

    List<Test> list(Map args)

    Long count()

    void delete(Serializable id)

    Test save(Test test)

}
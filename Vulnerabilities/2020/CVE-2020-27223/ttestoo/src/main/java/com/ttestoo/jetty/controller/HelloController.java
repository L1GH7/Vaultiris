package com.ttestoo.jetty.controller;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

/**
 * @Author 领悟。
 * @Date 2021/8/18 2:33 下午
 */

@RestController
@RequestMapping("/hello")
public class HelloController {

    @RequestMapping(value = "", method = RequestMethod.GET)
    public String hello() {
        return "hello";
    }


}

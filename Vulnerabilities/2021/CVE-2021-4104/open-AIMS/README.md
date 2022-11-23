# Log4j version 1.2.17-aims

This jar was created to protect against **CVE-2021-4104**.

It is meant to be used as a direct replacement for Log4j v1 libraries.

Log4j 1.2 project page: https://logging.apache.org/log4j/1.2/

Apache 2.0 Licence: https://www.apache.org/licenses/LICENSE-2.0

## Usage

Add this to your `pom.xml`

```
<project ...>
    <repositories>
        <!-- Open AIMS maven repository on GitHub -->
        <repository>
            <id>github_openaims</id>
            <name>GitHub Open-AIMS repo</name>
            <url>https://maven.pkg.github.com/open-AIMS/*</url>
        </repository>
    </repositories>

    ...

    <dependencies>
        <dependency>
            <groupId>au.gov.aims</groupId>
            <artifactId>log4j</artifactId>
            <version>1.2.17-aims</version>
        </dependency>

        ...
    </dependencies>
</project>
```

If one of the dependencies uses another version of log4j,
exclude it with the exclusions group in the dependency
declaration.

Example:
```
<project ...>
    ...

    <dependencies>
        <dependency>
            <groupId>au.gov.aims</groupId>
            <artifactId>log4j</artifactId>
            <version>1.2.17-aims</version>
        </dependency>

        <dependency>
            <groupId>uk.ac.rdg.resc</groupId>
            <artifactId>edal-xml-catalogue</artifactId>
            <version>1.2.4</version>
            <type>jar</type>

            <exclusions>
                <exclusion>
                    <groupId>log4j</groupId>
                    <artifactId>log4j</artifactId>
                </exclusion>
            </exclusions>

        </dependency>

        ...
    </dependencies>

</project>
```

If the project is managed by GitHub workflow, don't forget to add credentials for the
`github_openaims` repository in your `maven-settings.xml` file:
```
<settings ...>
    ...
    <servers>
        <!-- Used by GitHub server to resolve Open-AIMS dependencies when running tests or building the package -->
        <server>
            <id>github_openaims</id>
            <username>${env.GITHUB_USERNAME}</username>
            <password>${env.GITHUB_TOKEN}</password>
        </server>
    </servers>
</settings>
```

## Creation of the jar

How this jar was created:

### Copying the original files

Copy the jar from your local maven repo to this project:
```
$ cp ~/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar jar/log4j-1.2.17-aims.jar
```

### Modifying the JAR

Reference: https://access.redhat.com/security/cve/CVE-2021-4104

Remove the offending class:
```
$ zip -q -d jar/log4j-1.2.17-aims.jar org/apache/log4j/net/JMSAppender.class
```

### Deploy in Maven Open-AIMS as a MVN library

Create a new release on GitHub

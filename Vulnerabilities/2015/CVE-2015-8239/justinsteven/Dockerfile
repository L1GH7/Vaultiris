FROM debian:stable

RUN \
    apt-get update && \
    apt-get install -y \
        python3-pyinotify \
        sudo \
        tmux \
    && useradd -m editor && \
    useradd -m executor

COPY --chown=root:root hello /opt/hello
COPY --chown=root:root goodbye /opt/goodbye
COPY --chown=editor:editor hello /opt/sudoable
COPY --chown=editor:editor exploit /home/editor/exploit

COPY --chown=root:root sudoers_sudoable.tmpl /etc/sudoers.d/sudoable

RUN \
    sed -i "s/__HASH__/$(sha256sum /opt/sudoable | awk '{print $1}')/" /etc/sudoers.d/sudoable

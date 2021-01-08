FROM freehackquest/fhq-docker-build:latest
WORKDIR /root/

# Fix for building on debian 9.5 system (mysqlclient library)
# RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

COPY ./fhq-server /root/fhq-server
WORKDIR /root/fhq-server
RUN ./clean.sh && ./build_simple.sh

COPY ./user-web-site /root/user-web-site
WORKDIR /root/user-web-site
RUN npm install && npm run build-prod

FROM freehackquest/fhq-docker-common:latest

LABEL "maintainer"="Evgenii Sopov <mrseakg@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

COPY --from=0 /root/fhq-server/fhq-server /usr/bin/fhq-server
RUN mkdir -p /usr/share/fhq-server \
    && mkdir -p /usr/share/fhq-server/web/fhqjad-store \
    && mkdir -p /usr/share/fhq-server/public/games \
    && mkdir -p /usr/share/fhq-server/web/public/games \
    && mkdir -p /var/log/fhq-server
COPY ./admin-web-site /usr/share/fhq-server/admin-web-site
COPY --from=0 /root/user-web-site/dist /usr/share/fhq-server/user-web-site

# 1234 - ws
# 4613 - wss
# 7080 - http port (without ssl)

EXPOSE 1234 4613 7080
CMD fhq-server start

FROM freehackquest/fhq-docker-build:latest
WORKDIR /root/

# Fix for building on debian 9.5 system (mysqlclient library)
# RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

COPY ./fhq-server /root/fhq-server
WORKDIR /root/fhq-server
RUN ./clean.sh && ./build_simple.sh

COPY ./fhq-web-user /root/fhq-web-user
WORKDIR /root/fhq-web-user
RUN npm install && npm run build

FROM freehackquest/fhq-docker-common:latest

LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

COPY --from=0 /root/fhq-server/fhq-server /usr/bin/fhq-server
RUN mkdir -p /usr/share/fhq-server
COPY ./web-admin /usr/share/fhq-server/web-admin
COPY --from=0 /root/fhq-web-user/dist /usr/share/fhq-server/fhq-web-user
RUN mkdir -p /usr/share/fhq-server/public/games 
RUN mkdir -p /var/log/fhq-server

# 1234 - ws
# 4613 - wss
# 7080 - http port (without ssl)

EXPOSE 1234 4613 7080
CMD fhq-server start

FROM sea5kg/fhq-server-build-environment:latest
WORKDIR /root/

# Fix for building on debian 9.5 system (mysqlclient library)
# RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

# Build server application
COPY . /root/fhq-server
WORKDIR /root/fhq-server
RUN ./clean.sh && ./build_simple.sh

# Build web
COPY ./web-user /root/web-user
RUN cp -rf /root/node_modules_cache/node_modules /root/web-user
WORKDIR /root/web-user
RUN npm install && npm run build-prod

FROM sea5kg/fhq-server-release-environment:latest

LABEL "maintainer"="Evgenii Sopov <mrseakg@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

COPY --from=0 /root/fhq-server/fhq-server /usr/bin/fhq-server
RUN mkdir -p /usr/share/fhq-server \
    && mkdir -p /usr/share/fhq-server/web/fhqjad-store \
    && mkdir -p /usr/share/fhq-server/public/games \
    && mkdir -p /usr/share/fhq-server/web/public/games \
    && mkdir -p /var/log/fhq-server
COPY ./web-admin /usr/share/fhq-server/web-admin
COPY --from=0 /root/web-user/dist /usr/share/fhq-server/web-user

# 1234 - ws
# 4613 - wss
# 7080 - http port (without ssl)

EXPOSE 1234 4613 7080
CMD fhq-server start

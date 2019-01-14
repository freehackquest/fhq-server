FROM debian:9.5

RUN apt-get update && apt-get install -y \
    make cmake \
    g++ \
    pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
    default-libmysqlclient-dev
    
RUN apt-get install -y libqt5sql5-mysql libqt5websockets5 libqt5websockets5-dev \
    libwebsockets-dev make qt5-default qtchooser

# Fix for building on debian system (mysqlclient library)
RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

COPY ./fhq-server /root/sources
WORKDIR /root/sources
RUN ./clean.sh && ./build_simple.sh

FROM debian:9.5

LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

EXPOSE 1234 4613
COPY --from=0 /root/sources/fhq-server /usr/bin/fhq-server

RUN mkdir -p /usr/share/fhq-server/public/games 
RUN mkdir -p /var/log/fhq-server

RUN apt-get update && apt-get install -y libcurl3 zlibc zlib1g libpng16-16 libmariadbclient18

RUN apt-get install -y libqt5sql5-mysql
RUN apt-get install -y libqt5websockets5

# RUN apt-get install -y qt5-default qtchooser \

RUN apt-get clean

CMD fhq-server start
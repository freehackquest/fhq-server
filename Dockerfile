FROM debian:9.5

# basic libs

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    make cmake \
    g++ \
    pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
    default-libmysqlclient-dev \ 
    libwebsockets-dev \
    apt-utils

# install qt5\

RUN apt-get install -y \
    libqt5sql5-mysql \
    libqt5websockets5 \
    libqt5websockets5-dev \
    qt5-default \
    qtchooser

# Fix for building on debian system (mysqlclient library)
RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

COPY ./fhq-server /root/sources
WORKDIR /root/sources
RUN ./clean.sh && ./build_simple.sh

FROM debian:9.5

LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-server"

COPY --from=0 /root/sources/fhq-server /usr/bin/fhq-server
RUN mkdir -p /usr/share/fhq-server
COPY ./web-admin /usr/share/fhq-server/web-admin
RUN mkdir -p /usr/share/fhq-server/public/games 
RUN mkdir -p /var/log/fhq-server

RUN apt-get update && \
    apt-get install -y \
    libcurl3 \
    zlibc \
    zlib1g \
    libpng16-16 \
    libmariadbclient18 \
    libpthread-stubs0-dev \
    locales

# RUN locale-gen en_US.UTF-8
RUN sed -i -e "s/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/" /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
# RUN update-locale LANG=en_US.UTF-8

RUN apt-get install -y \
    libqt5sql5-mysql \
    libqt5websockets5 \
    libqt5core5a \
    libqt5concurrent5

# RUN apt-get install -y qt5-default qtchooser \

RUN apt-get clean

# 1234 - ws
# 4613 - wss
# 7080 - http port (without ssl)

EXPOSE 1234 4613 7080
CMD fhq-server start

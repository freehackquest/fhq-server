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

CMD cd /root/sources && ./clean.sh && ./build_simple.sh && mv fhq-server fhq-server.debian95
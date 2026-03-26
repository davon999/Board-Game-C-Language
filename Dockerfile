FROM ghcr.io/open-education-hub/vmcheker-next-base-image/base-container:latest
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Bucharest

##### PASS 1: Install system applications
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y locales && \
    locale-gen "en_US.UTF-8" && \
    dpkg-reconfigure locales

##### PASS 2: Install checker+debugging dependencies
RUN apt-get update -yqq \
    && apt-get install -yqq build-essential\
    && apt-get install -yqq valgrind \
    && apt-get install -yqq gdb
RUN apt-get install -yqq zip
##### PASS 3: Force image to have the unmodified checker
COPY ./checker ${CHECKER_DATA_DIRECTORY}
##### PASS 4: Overwrite main to avoid permission issues
COPY ./main.sh /base/main.sh
RUN chmod +x /base/main.sh

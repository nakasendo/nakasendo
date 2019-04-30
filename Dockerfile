# Use an official Python runtime as a parent image
FROM ubuntu:18.04

RUN apt-get update \
  && apt-get install -y git \
			gcc-6 \
			g++-6 \	
                        make \
                        wget \
			cmake \
			maven \
            ca-certificates \
            gosu \
            dirmngr \
            libboost-all-dev \
            libtool \
            libssl-dev \
            libzmq3-dev \
            python3

RUN apt-get update && apt-get install -y libcrypto++-dev \
		libcrypto++-utils \
		doxygen \
		libgmp3-dev \
		libkeyutils1 \
		vim \
		rpm


#update the memory settings according to the SDK source
RUN sed -i.bak '/# End of file/i root\t\thard\tmemlock\t\tunlimited\nroot\t\tsoft\tmemlock\t\tunlimited' /etc/security/limits.conf \
	&& sed -i.bak '$a kernel.keys.maxkeys = 25000\nkernal.keys.maxbytes = 2500000' /etc/sysctl.conf
# To solve add-apt-repository
RUN apt-get -y install software-properties-common

# install key utils
RUN cd /home && wget -O keyutils-1.5.10.tar.bz2  http://people.redhat.com/~dhowells/keyutils/keyutils-1.5.10.tar.bz2 \
	&& tar xvf keyutils-1.5.10.tar.bz2 \
	&& cd keyutils-1.5.10 \
	&& make \
	&& make install \
	&& cp /x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu

#ENV BOOST_ROOT=/opt/boost-169
#ENV BOOST_LIBRARYPATH=/opt/boost-169

# Download boost, untar, setup install with bootstrap and only do the Program Options library,
# and then install#
#RUN cd /home &&  wget -O boost_1_69_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.69.0/boost_1_69_0.tar.gz/download \
#  && tar xfz boost_1_69_0.tar.gz \
#  && rm boost_1_69_0.tar.gz \
#  && cd boost_1_69_0 \
#  && ./bootstrap.sh --prefix=/usr/boost-1.69 --with-libraries=all \
#  && ./b2 install \
#  && cd /home \
#  && rm -rf boost_1_69_0
#ARG repo_loc 

#COPY ${repo_loc}/* /home 


# Install any needed packages specified in requirements.txt
#RUN pip install --trusted-host pypi.python.org -r requirements.txt

# Run app.py when the container launches
#CMD ["bash"]
COPY ./entrypoint.sh .
RUN chmod +x /entrypoint.sh
RUN useradd -G users Jenkins
USER Jenkins


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/facerec_lbph_extended.cpp \
../src/histogram.cpp 

OBJS += \
./src/facerec_lbph_extended.o \
./src/histogram.o 

CPP_DEPS += \
./src/facerec_lbph_extended.d \
./src/histogram.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/Cellar/opencv/2.4.13/include -O0 -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



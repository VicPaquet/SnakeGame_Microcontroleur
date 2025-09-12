################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/NucleoImp/Display/ILI9341Display.cpp 

C_SRCS += \
../Core/Src/NucleoImp/Display/ili9341.c \
../Core/Src/NucleoImp/Display/ili9341_font.c \
../Core/Src/NucleoImp/Display/ili9341_gfx.c 

C_DEPS += \
./Core/Src/NucleoImp/Display/ili9341.d \
./Core/Src/NucleoImp/Display/ili9341_font.d \
./Core/Src/NucleoImp/Display/ili9341_gfx.d 

OBJS += \
./Core/Src/NucleoImp/Display/ILI9341Display.o \
./Core/Src/NucleoImp/Display/ili9341.o \
./Core/Src/NucleoImp/Display/ili9341_font.o \
./Core/Src/NucleoImp/Display/ili9341_gfx.o 

CPP_DEPS += \
./Core/Src/NucleoImp/Display/ILI9341Display.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/NucleoImp/Display/%.o Core/Src/NucleoImp/Display/%.su Core/Src/NucleoImp/Display/%.cyclo: ../Core/Src/NucleoImp/Display/%.cpp Core/Src/NucleoImp/Display/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/NucleoImp/Display/%.o Core/Src/NucleoImp/Display/%.su Core/Src/NucleoImp/Display/%.cyclo: ../Core/Src/NucleoImp/Display/%.c Core/Src/NucleoImp/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-NucleoImp-2f-Display

clean-Core-2f-Src-2f-NucleoImp-2f-Display:
	-$(RM) ./Core/Src/NucleoImp/Display/ILI9341Display.cyclo ./Core/Src/NucleoImp/Display/ILI9341Display.d ./Core/Src/NucleoImp/Display/ILI9341Display.o ./Core/Src/NucleoImp/Display/ILI9341Display.su ./Core/Src/NucleoImp/Display/ili9341.cyclo ./Core/Src/NucleoImp/Display/ili9341.d ./Core/Src/NucleoImp/Display/ili9341.o ./Core/Src/NucleoImp/Display/ili9341.su ./Core/Src/NucleoImp/Display/ili9341_font.cyclo ./Core/Src/NucleoImp/Display/ili9341_font.d ./Core/Src/NucleoImp/Display/ili9341_font.o ./Core/Src/NucleoImp/Display/ili9341_font.su ./Core/Src/NucleoImp/Display/ili9341_gfx.cyclo ./Core/Src/NucleoImp/Display/ili9341_gfx.d ./Core/Src/NucleoImp/Display/ili9341_gfx.o ./Core/Src/NucleoImp/Display/ili9341_gfx.su

.PHONY: clean-Core-2f-Src-2f-NucleoImp-2f-Display


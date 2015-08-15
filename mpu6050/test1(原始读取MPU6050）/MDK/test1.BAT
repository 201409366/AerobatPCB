SET PATH=C:\Keil\ARM\BIN40;C:\Program Files\NVIDIA Corporation\PhysX\Common;C:\Program Files\Common Files\NetSarang;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\MATLAB7\bin\win32;C:\Program Files\Microsoft SQL Server\90\Tools\binn\;C:\Program Files\MySQL\MySQL Server 5.1\bin
SET ARMCC41INC=C:\Keil\ARM\RV31\Inc
SET ARMCC41LIB=C:\Keil\ARM\RV31\Lib
SET CPU_TYPE=STM32F103VC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=test1
SET CPU_CLOCK=0x007A1200
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\main.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\core_cm3.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\system_stm32f10x.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\misc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_adc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_bkp.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_can.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_cec.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_crc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_dac.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_dbgmcu.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_dma.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_exti.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_flash.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_fsmc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_i2c.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_iwdg.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_pwr.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_rtc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_sdio.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_spi.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_tim.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_wwdg.__i"
"C:\Keil\ARM\BIN40\ArmAsm" --Via ".\obj\startup_stm32f10x_hd._ia"
"C:\Keil\ARM\BIN40\ArmLink" --Via ".\Obj\test1.lnp"
C:\Keil\ARM\BIN40\fromelf.exe ".\Obj\test1.axf" --i32combined --output ".\Obj\test1.hex"

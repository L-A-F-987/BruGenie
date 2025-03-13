ifneq ($(KERNELRELEASE),)
include Kbuild

else
KVERSION ?= `uname -r`
KDIR ?= /lib/modules/${KVERSION}/build
default:
	CONFIG_DRIVER_ST_VL53L4CD=m $(MAKE) -C $(KDIR) M=$(PWD)

clean:
	CONFIG_DRIVER_ST_VL53L4CD=m $(MAKE) -C $(KDIR) M=$(PWD) clean

dtb:
	sudo rm -f /boot/overlays/stmvl53l4cd.dtbo
	dtc -I dts -O dtb -o stmvl53l4cd.dtbo stmvl53l4cd.dts
	sudo cp stmvl53l4cd.dtbo /boot/overlays/stmvl53l4cd.dtbo

insert :
	sudo insmod stmvl53l4cd.ko

remove :
	sudo rmmod stmvl53l4cd
	
endif

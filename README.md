# Electronic Technologies and Biosensors Laboratory
## Academic Year 2021/2022 - II Semester

## Assignment
This repository contains the PSoC Creator workspace to be used as a starting point for the successfull submission of the Assignment
of the course "Electronic Technologies and Biosensors Laboratory" of the II Semester of the academic year 2021/2022.

### Objective
In this Project, you have to setup an I2C Slave using the PSoC and sample 2 analog sensors using a Delta-Sigma ADC. Moreover, you have to modulate a RGB LED according to the output of one sensor. For data visualization and interaction, you will use the Bridge Control Panel to
plot the two signals.

### Connections

Pin          | Description |
:-----------:|-------------|
2.2          | R channel of the RGB LED
2.3          | G channel of the RGB LED
2.4          | B channel of the RGB LED
2.5          | Analog Channel 0: TMP (Temperature sensor) (\*)
2.6          | Analog Channel 1: LDR (Light Dependent Resistor) (\**)
12.0         | I2C SCL
12.1         | I2C SDA

(*) See the connection scheme below to hook up the photoresistor (<a href="https://components101.com/resistors/ldr-datasheet">Datasheet</a>)
<a href="https://ibb.co/Vm0pbTk"><img src="https://i.ibb.co/tXwPnD1/Cattura.jpg" alt="Photoresistor schematic" width="70%" border="0"></a>

(\**) Please refer to the following image and to the datasheet of the component (<a href="https://www.analog.com/en/products/tmp36.html">TMP36GZ</a>) to hook up the temperature sensor

<a href="https://imgbb.com/"><img src="https://i.ibb.co/BcVt5BN/Untitled-Diagram.jpg" alt="Untitled-Diagram" border="0"></a>

### Requirements
The Project will have to meet the following requirements:
- Sample two analog sensors using a single 16-bit Delta-Sigma ADC (hint: you can use the <a href="https://www.cypress.com/file/127201/download">Analog Mux</a> for this)
- Each sample (of each channel) has to be the average of **X** consecutive samples buffered opportunely (refer to the table below)
- Set up an I2C Slave (<a href="https://www.cypress.com/file/185396/download">EZI2C</a>) to send the averaged data for the 2 channels to the Bridge Control Panel. 
  Configure the EZI2C component as follows: 100 kbps data rate, 1 address (0x08)
- The required transmission data rate is **50 Hz**
- Set up a Timer with an ISR at the appropriate frequency to guarantee the data transmission rate (50 Hz) according to the number of averaged sample (1-8 samples)
- Control the operation of the device writing the value of the Control Register of the I2C slave (refer to the table below)
- Set up the I2C Slave buffer of the EZI2C according to the following table (set the WHO AM I to 0xBC)
- Modulate the intensity of the RGB LED depending on the measurment of the LDR or TMP sensor (according to status register). 
  - When the LED is modulated by the LDR and the environment is dark, the LED must be switched on with maximum intensity, while when the environment is bright the LED must be switched off. **Note**: the LDR characteristic curve is very steep (almost ON/OFF), please consider this fact in order to better exploit the sensor dynamic, either hardware or firmware solution.
  - When the LED is modulated by the TMP, the LED must be switched off at ambient temperature and increase intensity with rising temperature (e.g., try to breath on the sensor or press your hand on it)


**I2C Slave Buffer Structure**

Address      | Description   | R/W |
:-----------:|---------------|:---:|
0x00         | Control Reg 1 | R/W |
0x01         | Who Am I      |  R  |
0x02         | LDR Bit 15-8  |  R  |
0x03         | LDR Bit 07-0  |  R  |
0x04         | TMP Bit 15-8  |  R  |
0x05         | TMP Bit 07-0  |  R  |


**Control Register 1 Description** (Address 0x00)

Bit    | Description           | R/W | Default |
:-----:|-----------------------|:---:|:-------:|
7      | LED B channel         | R/W |    0    |
6      | LED G channel         | R/W |    0    |
5      | LED R channel         | R/W |    0    |
4      | Average samples bit 1 | R/W |    0    |
3      | Average samples bit 0 | R/W |    0    |
2      | LED modality          | R/W |    0    |
1      | TMP status            | R/W |    0    |
0      | LDR status            | R/W |    0    |

The `status` (bits 0 and 1) can be either set to `0b00` (device stopped), `0b01` to sample the first channel (LDR), `0b10` to sample the second channel (TMP), and finally `0b11` to sample both channels.

The `LED modality` (bit 2) can be set to choose between the sensor that modulates RGB LED intensity. Choose `0` to exploit LDR readout or `1` to use TMP readout.

The register contains also the number of samples to be used for the 
computation of the average. Set the value of bits 3-4 of the Control Register 1 to meet the requirements of the project. The minimum number of sample to be averaged is 1 (`0b000`) and its maximum is 4 (`0b11`).

**Note**: mind that the required transmission data rate (`50 Hz`) may differ from the ISR frequency of the timer (because of the average computation). You have to set up the ISR accordingly to guarantee the required transmission data rate.

The `RGB channel` (bits 5-7) can be set to choose the output color combination. Remember that the intensity is modulated by the LDR, with this bits you only set which channel you want to switch on (either single channel or combination of them).

### Setup and Assignment Delivery
- One student from the team forks this repository 
- Following the forking of the repository, the other team member(s) are added as collaborators via GitHub
- Each team member clones the forked repository by entering the following command in the terminal:

    `git clone https://github.com/[your_username]/AY2122_II_HW.git`
- Open up the workspace in PSoC Creator
- Activate the project with your group number
- Work on your project
- In your Group folder, save the Bridge Control Panel configuration files (GROUP_XX.ini and GROUP_XX.iic) inside BRIDGE_CONTROL_PANEL_CONFIG_FILES folder  
- The last commit must be named '*GROUP_XX* Final Delivery', where XX is the group number
- One member of the team proposes a pull request before the deadline for the assignment, which is on **April 29 at 6PM**


### Evaluation
The evaluation of the assignment will take into consideration the following aspects of the project:
- successful build process without any warning/error generated
- successful programming without any error generated
- correct functioning of the device as per the requirements
- code organization (header and source files, useful comments)
- schematic organization and documentation
- successful use of git for version control (use of branches, ...)
- successful collaboration with git across group members
- successful pull request on GitHub

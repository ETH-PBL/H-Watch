<!--
*** Template source: https://github.com/othneildrew/Best-README-Template/blob/master/README.md
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/ETH-PBL/H-Watch">
    <img src="img/abstract.png" alt="Logo" width="1269" height="325">
  </a>

  <h3 align="center">H-Watch</h3>

  <p align="center">
    Smart Watch for Healthcare
    <br />
    <a href="https://github.com/ETH-PBL/H-Watch"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://www.youtube.com/watch?v=JSp1-hzvBWk">View Demo</a>
    ·
    <a href="https://github.com/ETH-PBL/H-Watch/issues">Report Bug</a>
    ·
    <a href="https://github.com/ETH-PBL/H-Watch/issues">Request Feature</a>
  </p>
</p>


<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project


### Built With
The H-Watch is a highly integrated multi-sensor wearable platform which is built of the key components, listed below: 

* [MAX30101EFD+][max301010_url],  High-Sensitivity Pulse Oximeter and Heart-Rate Sensor for Wearable Health
* [LSM303AGR][lsm303agr_url],     Ultra-compact high-performance eCompass module
* [LPS22HB][lps22hb_url],         MEMS nano pressure sensor
* [LSM6DSM][lsm6dsm_url],         Ultra-low power, high accuracy and stability iNEMO 6DoF inertial measurement unit (IMU)
* [MP34DT05-A][mp34dt05_url],     MEMS audio sensor omnidirectional stereo digital microphone
* [BQ25570][bq25570_url],         Ultra Low power Harvester power Management IC with boost charger, and Nanopower Buck Converter
* [STM32WB55RG][stm32wb55_url],   Ultra-low-power dual core Arm Cortex-M4 MCU 64 MHz, Cortex-M0+ 32MHz with 1 Mbyte of Flash memory, Bluetooth LE 5.2
* [BC95GJB-02-STD][bc95G_url],    Multi-band NB-IoT Module with Ultra-low Power Consumption



<!-- GETTING STARTED -->
## Getting Started

This project is mainly compoed by three subsystems: the UAVs, the Wireless Transceiver, and the Sensor Node. For each subsystem we propose the source code and the hardware platform, including results and simulations.

### Prerequisites

#### Hardware

* Sensor node, not yet available from commercial stores.

<p align="center">
  <a href="https://github.com/tommasopolonelli/SynthSense-WSN-UAV">
    <img src="img/sno.png" alt="Logo" width="1269" height="325">
  </a>
</p>

- [UWB antenna WB002](https://www.decawave.com/uwb-antennae-design-files/)
*[Decawave EVK1000](https://www.decawave.com/product/evk1000-evaluation-kit/)
* [STM32F407G-DISC1](https://www.st.com/en/evaluation-tools/stm32f4discovery.html)
* [DJI Matrice 100](https://www.dji.com/it/matrice100)
* [Micro SD adapter](https://www.digikey.it/product-detail/it/adafruit-industries-llc/254/1528-1462-ND/5761230)
* [USB-UART adapter](https://it.aliexpress.com/item/2024415412.html)
* Micro SD Card (min 2 GB)

<p align="center">
  <a href="https://github.com/tommasopolonelli/SynthSense-WSN-UAV">
    <img src="img/dji_EVK1000.jpg" alt="Logo" width="1269" height="1269">
  </a>
</p>



#### Software

### Installation









<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[max301010_url]:    https://www.maximintegrated.com/en/products/interface/sensor-interface/MAX30101.html
[lsm303agr_url]:    https://www.st.com/en/mems-and-sensors/lsm303agr.html
[lps22hb_url]:      https://www.st.com/en/mems-and-sensors/lps22hb.html
[lsm6dsm_url]:      https://www.st.com/en/mems-and-sensors/lsm6dsm.html
[mp34dt05_url]:     https://www.st.com/en/mems-and-sensors/mp34dt05-a.html
[bq25570_url]:      https://www.ti.com/product/BQ25570
[stm32wb55_url]:    https://www.st.com/en/microcontrollers-microprocessors/stm32wb55rg.html
[bc95G_url]:        https://www.quectel.com/UploadFile/Product/Quectel_BC95-G_NB-IoT_Specification_V1.2.pdf


[contributors-shield]: https://img.shields.io/github/contributors/tommasopolonelli/SynthSense-WSN-UAV.svg?style=flat-square
[contributors-url]: https://github.com/tommasopolonelli/SynthSense-WSN-UAV/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/tommasopolonelli/SynthSense-WSN-UAV.svg?style=flat-square
[forks-url]: https://github.com/tommasopolonelli/SynthSense-WSN-UAV/network/members
[stars-shield]: https://img.shields.io/github/stars/tommasopolonelli/SynthSense-WSN-UAV.svg?style=flat-square
[stars-url]: https://github.com/tommasopolonelli/SynthSense-WSN-UAV/stargazers
[issues-shield]: https://img.shields.io/github/issues/tommasopolonelli/SynthSense-WSN-UAV.svg?style=flat-square
[issues-url]: https://github.com/tommasopolonelli/SynthSense-WSN-UAV/issues
[license-shield]: https://img.shields.io/github/license/tommasopolonelli/SynthSense-WSN-UAV.svg?style=flat-square
[license-url]: https://github.com/tommasopolonelli/SynthSense-WSN-UAV/blob/master/LICENSE
[product-screenshot]: images/screenshot.png

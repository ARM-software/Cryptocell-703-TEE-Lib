/*!

   @mainpage Arm® CryptoCell-703™ Runtime Software API overview

    This documentation describes the runtime software APIs provided by Arm CryptoCell-703.
    It provides you with all the information necessary for integrating and using
    the runtime software APIs in the target environment.

    The API layer enables using the CryptoCell cryptographic algorithms, such as SM2, SM3 and SM4.

    The API layer enables use of the following algorithms and features:
    <ul>
        <li>Public Key Cryptographic Algorithm SM2 Based on Elliptic Curves</li>
        <li>SM3 Cryptographic Hash Algorithm</li>
        <li>SM4 Cryptographic Block Cipher</li>
        <li>True Random Number generator</li>
        <li>Content Protection Policy keys</li>
        <li>Power management</li>
    </ul>
  This documentation is automatically generated from the source code using Doxygen.
  For more information on Doxygen, see:
    http://www.doxygen.nl/

 The <i>Modules</i> section introduces the high-level module concepts used throughout this documentation.

  @section conf_status Confidentiality status

    \htmlonly
      <p overflow="hidden">
          <embed src="conf_status.html" type='text/html' width="80%" height="50px">
      </p>
    \endhtmlonly

  @section proprietary_notice Proprietary notice

    \htmlonly
      <p overflow="hidden">
          <embed type='text/html' src="proprietary_notice.html" width="80%" height="700px">
      </p>
    \endhtmlonly

 @section add_read Additional reading

    The Software Developer Manual contains information that is specific to this product. See the following documents
    for other relevant information:
    \htmlonly
    <table class="bordered">
      <caption align="left">Arm publications</caption>
      <tr>
        <th>Document name</th>
        <th>Document ID</th>
        <th>Licensee only Y/N</th>
      </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> AMBA® AXI and ACE Protocol Specification</cite>, February 2013 </td>
            <td>IHI 0022F</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> Trusted Base System Architecture V1: System Software on Arm</cite></td>
            <td>DEN 0007C</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> Power State Coordination Interface Platform Design Document</cite> </td>
            <td>DEN 0022D</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> AMBA®3 APB Protocol Specification</cite>, April 2010</td>
            <td>IHI 0024C</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> AMBA® Low Power Interface Specification</cite>, September 2016</td>
            <td>IHI 0068C</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> Trusted Boot Board Requirements: System Software on Arm</cite></td>
            <td>DEN 0006C-1</td>
            <td>No</td>
        </tr>
        <tr>
            <td><cite>Arm® <span class="keyword">CryptoCell™-703</span> Technical Reference Manual</cite></td>
            <td>Arm 101352</td>
            <td>Yes</td>
        </tr>
        <tr>
            <td><cite>Arm® <span class="keyword">CryptoCell™-703</span> Configuration and Integration Manual</cite> </td>
            <td>Arm 101353</td>
            <td>Yes</td>
        </tr>
        <tr>
            <td><cite>Arm® <span class="keyword">CryptoCell™-703</span> Software Integrators Manual</cite></td>
            <td>Arm 101509</td>
            <td>Yes</td>
        </tr>
        <tr>
            <td><cite>Arm® <span class="keyword">CryptoCell™-703</span> Software Release Notes</cite></td>
            <td>PJDOC-1779577084-12531</td>
            <td>Yes</td>
        </tr>
        <tr>
            <td><cite><span class="keyword">Arm®</span> TRNG Characterization Application Note</cite> </td>
            <td>Arm 100685</td>
            <td>Yes</td>
        </tr>
    </table>
    <table class="bordered">
           <caption align="left">Other publications</caption>
        <tr>
            <th>Document name</th>
            <th>Document ID</th>
        </tr>
           <tr>
            <td>AIB20</td>
            <td><cite>Functionality classes and evaluation methodology for deterministic random number generators</cite></td>
        </tr>
        <tr>
            <td>-</td>
            <td><cite>ChinaDRM Compliance Rules and Robustness Rules, December 2016</cite></td>
        </tr>
        <tr>
            <td>-</td>
            <td><cite>ChinaDRM lab: A description of ChinaDRM implementation (2016)</cite></td>
        </tr>
        <tr>
            <td>BSI AIS-31 </td>
            <td><cite>Functionality Classes and Evaluation Methodology for True Random Number Generators</cite></td>
        </tr>
        <tr>
            <td>FIPS Publication 140IG </td>
            <td><cite>Implementation Guidance for FIPS PUB 140-2 and the Cryptographic Module Validation Program (November 2015)</cite></td>
        </tr>
        <tr>
            <td>GM/T 0005-2012</td>
            <td><cite>Randomness Test Specification</cite></td>
        </tr>
        <tr>
            <td>GM/T 0009-2012 </td>
            <td><cite>SM2 Cryptography Algorithm Application Specification</cite></td>
        </tr>
        <tr>
            <td>GM/T 0010-2012 </td>
            <td><cite>SM2 Cryptography Message Syntax Specification</cite></td>
        </tr>
        <tr>
            <td>GY/T 277—2014 </td>
            <td><cite>Technical specification of digital rights management for internet television (May 2014)</cite></td>
        </tr>
        <tr>
            <td>NIST SP 800-22 </td>
            <td><cite>A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications</cite></td>
        </tr>
        <tr>
            <td>PKCS #7 v1 </td>
            <td><cite>Public-Key Cryptography Standards Cryptographic Message Syntax Standard</cite></td>
        </tr>
        <tr>
            <td>RFC 4106</td>
            <td><cite>The Use of Galois/Counter Mode (GCM) in IPsec Encapsulating Security Payload (ESP)</cite></td>
        </tr>
        <tr>
            <td>RFC 4543</td>
            <td><cite>The Use of Galois Message Authentication Code (GMAC) in IPsec ESP and AH</cite></td>
        </tr>
        <tr>
            <td>RFC 5280 </td>
            <td><cite>Internet X.509 Public Key Infrastructure Certificate and Certificate Revocation List (CRL) Profile</cite></td>
        </tr>
        <tr>
            <td>SM2 </td>
            <td><cite>Public Key Cryptographic Algorithm Based on Elliptic Curves (December, 2010)</cite></td>
        </tr>
        <tr>
            <td>SM3 </td>
            <td><cite>Cryptographic Hash Algorithm (December 2012)</cite></td>
        </tr>
        <tr>
            <td>SM4</td>
            <td><cite>Security of the SMS4 Block Cipher Against Differential Cryptanalysis</cite></td>
        </tr>

    </table>



    \endhtmlonly

    @section glossary Glossary

    The Arm Glossary is a list of terms used in Arm documentation, together with definitions for those terms. The Arm Glossary does not contain terms that are industry standard unless the Arm meaning differs from the generally accepted meaning.

    See https://developer.arm.com/glossary for more information.

    \htmlonly
    <table class="bordered">
      <caption align="left">Terminology</caption>
        <tr>
            <td>CCI</td>
            <td>Cache Coherent Interconnect</td>
        </tr>
        <tr>
            <td>CPP</td>
            <td>Content Protection Policy.</td>
        </tr>
        <tr>
            <td>HAL</td>
            <td>Hardware Abstraction Layer</td>
        </tr>
        <tr>
            <td>Host</td>
            <td>SoC processor</td>
        </tr>
        <tr>
            <td>ICV</td>
            <td>Integrated Chip Vendor</td>
        </tr>
        <tr>
            <td>IRR</td>
            <td>Interrupt Register</td>
        </tr>
        <tr>
            <td>ISR</td>
            <td>Interrupt Service Routine </td>
        </tr>
        <tr>
            <td>OEM</td>
            <td>Original Equipment Manufacturer </td>
        </tr>
        <tr>
            <td>PAL</td>
            <td>Platform Abstraction Layer</td>
        </tr>
        <tr>
            <td>PKA</td>
            <td>Public Key Accelerator</td>
        </tr>
        <tr>
            <td>PM</td>
            <td>Power Management</td>
        </tr>
        <tr>
            <td>PMU</td>
            <td>Power Management Unit</td>
        </tr>
        <tr>
            <td>PoR</td>
            <td>Power-on Reset</td>
        </tr>
        <tr>
            <td>REE</td>
            <td>Rich Execution Environment </td>
        </tr>
        <tr>
            <td>SCU</td>
            <td>Snoop Control Unit</td>
        </tr>
        <tr>
            <td>SOS</td>
            <td>Secure Operating System</td>
        </tr>
        <tr>
            <td>SWCC</td>
            <td>Software Cache Coherency</td>
        </tr>
        <tr>
            <td>HWCC</td>
            <td>Hardware Cache Coherency</td>
        </tr>
        <tr>
            <td>TEE</td>
            <td>Trusted Execution Environment</td>
        </tr>
    </table>
    \endhtmlonly

 */

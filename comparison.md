---
title: Comparison with PHP GbxRemote | Mania++
---
[«« Index](index.html)

# Comparison with PHP GbxRemote #
Most popular controllers use the PHP version of the GbxRemote. This is included in the dedicated server package and most often the starting point of a (new) controller.
It's up to the C++ controller/library to at least stand up to this library. To check this out, a series of tests have been executed.

## Retrieving map list remotely ##
In this scenario both set-ups create a connection remotely (from inside the development-VM) to the production server the testserver is running on. The connection is 70 Mbps down/60 Mbps up and is stable around 16.3ms ping.
This is about retrieving the data and convert it to usable arrays/structs (no conversion to a map object is executed). The maplist contains 812 maps.

<table>
  <tr>
    <td></td>
    <td><strong>C++ (Mania++)</strong></td>
    <td><strong>PHP (GbxRemote)</strong></td>
  </tr>
  <tr>
    <td><strong>OS</strong></td>
    <td colspan="2">Ubuntu 16.10 64-bit</td>
  </tr>
  <tr>
    <td><strong>Kernel</strong></td>
    <td colspan="2">Linux 4.8.0-30</td>
  </tr>
  <tr>
    <td><strong>Compiler</strong></td>
    <td>GCC 6.2.0</td>
    <td>PHP 7.0.8</td>
  </tr>
  <tr><td colspan="3"></td></tr>
  <tr>
    <td><strong>#1</strong></td>
    <td>1.60555 sec.</td>
    <td>1.62182 sec.</td>
  </tr>
  <tr>
    <td><strong>#2</strong></td>
    <td>1.60041 sec.</td>
    <td>1.59549 sec.</td>
  </tr>
  <tr>
    <td><strong>#3</strong></td>
    <td>1.60527 sec.</td>
    <td>1.60514 sec.</td>
  </tr>
  <tr>
    <td><strong>#4</strong></td>
    <td>1.60996 sec.</td>
    <td>1.59953 sec.</td>
  </tr>
  <tr>
    <td><strong>#5</strong></td>
    <td>1.60145 sec.</td>
    <td>1.65126 sec.</td>
  </tr>
  <tr><td colspan="3"></td></tr>
  <tr>
    <td><strong>Total</strong></td>
    <td>8.02264 sec.</td>
    <td>8.07324 sec.</td>
  </tr>
  <tr>
    <td><strong>Average</strong></td>
    <td>1.60453 sec.</td>
    <td>1.61465 sec.</td>
  </tr>
  <tr>
    <td><strong>Difference</strong></td>
    <td colspan="2">0.01012 sec. in favour of Mania++</td>
  </tr>
</table>

## Retrieving map list locally ##
This scenario is similar to the previous one, except that in this one the controller(s) are connecting locally, removing the networking-factor from the equation.
Both language/compiler versions are older as this test is executed on a production server and software versions usually lag behind in that case. Note though that both versions are the latest available (in the series, f.e. PHP 5).

<table>
  <tr>
    <td></td>
    <td><strong>C++ (Mania++)</strong></td>
    <td><strong>PHP (GbxRemote)</strong></td>
  </tr>
  <tr>
    <td><strong>OS</strong></td>
    <td colspan="2">Debian 8.6</td>
  </tr>
  <tr>
    <td><strong>Kernel</strong></td>
    <td colspan="2">Linux 3.16.0-4</td>
  </tr>
  <tr>
    <td><strong>Compiler</strong></td>
    <td>GCC 4.9.2</td>
    <td>PHP 5.6.27</td>
  </tr>
  <tr><td colspan="3"></td></tr>
  <tr>
    <td><strong>#1</strong></td>
    <td>1.50472 sec.</td>
    <td>1.71565 sec.</td>
  </tr>
  <tr>
    <td><strong>#2</strong></td>
    <td>1.53870 sec.</td>
    <td>1.65651 sec.</td>
  </tr>
  <tr>
    <td><strong>#3</strong></td>
    <td>1.53649 sec.</td>
    <td>1.66803 sec.</td>
  </tr>
  <tr>
    <td><strong>#4</strong></td>
    <td>1.47690 sec.</td>
    <td>1.71096 sec.</td>
  </tr>
  <tr>
    <td><strong>#5</strong></td>
    <td>1.52664 sec.</td>
    <td>1.67395 sec.</td>
  </tr>
  <tr><td colspan="3"></td></tr>
  <tr>
    <td><strong>Total</strong></td>
    <td>7.58345 sec.</td>
    <td>8.42510 sec.</td>
  </tr>
  <tr>
    <td><strong>Average</strong></td>
    <td>1.51669 sec.</td>
    <td>1.68502 sec.</td>
  </tr>
  <tr>
    <td><strong>Difference</strong></td>
    <td colspan="2">0.16833 sec. in favour of Mania++</td>
  </tr>
</table>

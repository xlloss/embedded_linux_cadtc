--TEST--
time_sleep_until() function - error test for time_sleep_until()
--CREDITS--
Filippo De Santis fd@ideato.it
#PHPTestFest Cesena Italia on 2009-06-20
--FILE--
<?php
  var_dump(time_sleep_until('goofy'));
?>
--EXPECTF--
Warning: time_sleep_until() expects parameter 1 to be double, string given in %s on line 2
NULL

#!/usr/bin/perl

$counter_limit = 30;
$counter = 0;

while( $counter <= $counter_limit )
{
  system( "hamachi logout" );
  sleep 4;
  
  $result = system( "hamachi login" );
  if( 0 == $result )
  {
    last;
  }
  
  $counter++;
}


if( $counter > $counter_limit )
{
  system( "reboot" );
} 



#!/usr/bin/perl

system("hamachi logout");
sleep 4;
system("hamachi login");

while( 1 )
{
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

 sleep 3600
}


#!usr/bin/perl
my $configFile = $ARGV[0];
my $DRM_VALUE = $ARGV[1];
my $KEY_VALUE = $ARGV[2];
my $KEY_VALUE2 = $ARGV[3];
die "the file $configFile is NOT exsit\n" if ( ! -e $configFile);
open FILE, "<$configFile";
my @lines;
my $VALUE = yes;
while (<FILE>) {
	print "$_";
	if (/^(\S+)\s*=\s*(\S+)/) {
		if(!($1 eq "$KEY_VALUE")){
			@lines=(@lines,$_);
			if ($1 eq $KEY_VALUE2){
				$VALUE = $2;
			}
		}
	}else{
		@lines=(@lines,$_);
	}
}
close FILE;

my $filedir = "mediatek/config/out/$FULL_PROJECT";
my $write_filename = "$filedir/ProjectConfigtmp.mk";

system("touch $write_filename") if (!-e $write_filename);
system("chmod u+w $write_filename") if (-e $write_filename);
system("mkdir -p $filedir") if ( ! -d "$filedir");
die "can NOT open $write_filename:$!" if ( ! open OUT_FILE, ">$write_filename");

foreach my $option (@lines) {	
	print OUT_FILE "$option";
}

if($VALUE eq "no"){

	print OUT_FILE "MTK_GEMINI_3G_SWITCH=no\n";
}else{

	if($DRM_VALUE eq "yes"){
		print OUT_FILE "MTK_GEMINI_3G_SWITCH=yes\n";
	}elsif($DRM_VALUE eq "no"){
		print OUT_FILE "MTK_GEMINI_3G_SWITCH=no\n";
	}
}
close OUT_FILE;
system("mv $write_filename $configFile");


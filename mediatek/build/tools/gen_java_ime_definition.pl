#!usr/bin/perl


my %ime_dependency = (
	'MTK_IME_ENGLISH_SUPPORT' => 'en',
	'MTK_IME_PINYIN_SUPPORT'  => 'zh',
	'MTK_IME_ZHUYIN_SUPPORT'  => 'zh',
	'MTK_IME_STROKE_SUPPORT'  => 'zh',
	'MTK_IME_HANDWRITING_SUPPORT'=>'zh',);


my @Android_support_locales = (
    en_GB,
    en_SG,
    zh_CN,
    en_US,
    en_AU,
    zh_TW,
    en_NZ,
    fr_CA,
    nl_BE,
    fr_BE,
    de_DE,
    de_CH,
    fr_CH,
    it_CH,
    de_LI,
    nl_NL,
    pl_PL,
    ja_JP,
    fr_FR,
    ko_KR,
    es_ES,
    de_AT,
    it_IT,
    ru_RU,
    cs_CZ);


# foreach (@ARGV) { print $_,  " "; };  print "\n";
my $configOpt = $ARGV[0];  #project file name, such as mtk/make/oppo.mak
print "configOpt=$ARGV[0]\n";
my $project;

#* combile source file name, such as build/target/product/oppo.mk *#
$configPath="build/target/product/";

$suffix    =".mk";

if ($configOpt =~m/.*\/(\w+)(\[\w+\])?\/(\w+).mk/) {
    $project = $1;
} else { 
    die "project error";
}
# $project = "zte73v1_2";
$configFile=$configPath.$project.$suffix;  
#*** parsing PRODUCT_LOCALES in build/target/product/xxxx.mk **#
my $findbegin = 0;
my $findend   = 0;
my @locales_original;
my @locales_filtered;
my @ltemp;

die "the file $configFile is NOT exsit\n" if ( ! -e $configFile);
open (CFGFILE, "<$configFile") || die "Cann't open the file $!\n";

while (<CFGFILE>) {
	if (m/PRODUCT_LOCALES\s*:=\s*(.*)\\/) {
		@ltemp = split(/ /, $1);
		$findbegin = 1;
		foreach my $w (@ltemp) {
			push @locales_original, $w;
		}
		while (<CFGFILE>) {
			if (/\s(.*)\\/) {
				@ltemp = split(/ /, $1);
				foreach my $w (@ltemp) {
					push @locales_original, $w;
				}				
			}
			elsif (/\s*(.*)\s*/) {
			    @ltemp = split(/ /, $1);
				foreach my $w (@ltemp) {
					push @locales_original, $w;
				}
				$findend = 1;
			}
			if ($findend == 1) {
			    last;
			}			
		}
		$linefound = 0;
	}
	elsif (m/PRODUCT_LOCALES\s*:=\s*(.*)/) {
			@locales_original = split(/ /, $1);
			$findbegin = 1;
			$findend = 1;		
	}
	if ($findbegin == 1 && $findend == 1) {
	    last;
	}
}
close(CFGFILE);

#* filter hdpi, mdpi, ldpi, and remove space  *#
foreach (@locales_original) {
	if (! m/\s*\wdpi\s*/){
		if (m/\s*(\w+)\s*/) {
			push @locales_filtered, $1;
		}
	} 
}

#** write PRODUCT_LOCALES to file **# 

my $write_filename = "mediatek/source/frameworks/featureoption/java/com/mediatek/featureoption/IMEFeatureOption.java";

die "can NOT open $write_filename:$!" if ( ! open OUT_FILE, ">$write_filename");
print OUT_FILE "\/* generated by mediatek *\/\n\n";
print OUT_FILE "package com.mediatek.featureoption;\n";
print OUT_FILE "\npublic final class IMEFeatureOption\n{\n";
print OUT_FILE "\n\tpublic static final String[] PRODUCT_LOCALES=\n\t{\n";
foreach my $option (@locales_filtered) {
	print OUT_FILE "\t\t\"${option}\",\n";
}
print OUT_FILE "\t};\n";


my %locales_opt = ();
my %locales_std = ();
# Add all locales which Android can supports. 2 means false in java.
foreach my $w (@Android_support_locales) {
	if ($w=~m/(\w+)_(\w+)/) {
		my $temp, $lang, $lcon; 

		$locales_std{$1}=2;
		$locales_std{$w}=2;

		$temp = $1;
		$temp=~tr/a-z/A-Z/;
		$lang = "SYS_LOCALE_".$temp;
		$temp = $2;
		$temp=~tr/a-z/A-Z/;
		$lcon = $lang."_".$temp;
		$locales_opt{$lang}=2;
		$locales_opt{$lcon}=2;
	}
}
# Add locales which this project can supports. 1 means true in java.
foreach my $w (@locales_filtered) {
	if ($w=~m/(\w+)_(\w+)/) {
		my $temp, $lang, $lcon; 

		$locales_std{$1}=1;
		$locales_std{$w}=1;

		$temp = $1;
		$temp=~tr/a-z/A-Z/;
		$lang = "SYS_LOCALE_".$temp;
		$temp = $2;
		$temp=~tr/a-z/A-Z/;
		$lcon = $lang."_".$temp;
		$locales_opt{$lang}=1;
		$locales_opt{$lcon}=1;
	}
}
#my @sorted_locales=();
#@sorted_locales = sort {$a cmp $b;} keys %locales3;
foreach (keys %locales_opt) {
    if ( $locales_opt{$_} == 1) {
	    print OUT_FILE "\tpublic static final boolean \t$_ = \ttrue;\n";
}
}
foreach (keys %locales_opt) {
	if ( $locales_opt{$_} == 2) {
        print OUT_FILE "\tpublic static final boolean \t$_ = \tfalse;\n";
	}
}

print OUT_FILE "\n";

#* parsing MTK IME configuration, 1st->project file, 2nd->MTK_IME_SUPPORT, following... *#

#my @ime_opt_arr = @ARGV;

#######################################################
# this is a workaround, maybe fix in future
######################################################
my @ime_opt_arr;

if ($ENV{MTK_IME_RUSSIAN_SUPPORT} eq "yes")
{
       push @ime_opt_arr, "MTK_IME_RUSSIAN_SUPPORT = true";
}
if ($ENV{MTK_IME_RUSSIAN_SUPPORT} eq "no")
{
       push @ime_opt_arr, "MTK_IME_RUSSIAN_SUPPORT = false";
}
if ($ENV{MTK_IME_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_SUPPORT = true";
}
if ($ENV{MTK_IME_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_SUPPORT = false";
}
if ($ENV{MTK_IME_ENGLISH_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_ENGLISH_SUPPORT = true";
}
if ($ENV{MTK_IME_ENGLISH_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_ENGLISH_SUPPORT = false";
}
if ($ENV{MTK_IME_PINYIN_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_PINYIN_SUPPORT = true";
}
if ($ENV{MTK_IME_PINYIN_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_PINYIN_SUPPORT = false";
}
if ($ENV{MTK_IME_STROKE_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_STROKE_SUPPORT = true";
}
if ($ENV{MTK_IME_STROKE_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_STROKE_SUPPORT = false";
}
if ($ENV{MTK_IME_HANDWRITING_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_HANDWRITING_SUPPORT = true";
}
if ($ENV{MTK_IME_HANDWRITING_SUPPORT} eq "no")
{ 
	push @ime_opt_arr, "MTK_IME_HANDWRITING_SUPPORT = false";
}
 
if ($ENV{MTK_IME_ZHUYIN_SUPPORT} eq "yes")
{ 
	push @ime_opt_arr, "MTK_IME_ZHUYIN_SUPPORT = true";
}
if ($ENV{MTK_IME_ZHUYIN_SUPPORT} eq "no")
{ 
	push @ime_opt_arr, "MTK_IME_ZHUYIN_SUPPORT = false";
}

if ($ENV{MTK_IME_FRENCH_SUPPORT} eq "yes")
{
        push @ime_opt_arr, "MTK_IME_FRENCH_SUPPORT = true";
}
if ($ENV{MTK_IME_FRENCH_SUPPORT} eq "no")
{
        push @ime_opt_arr, "MTK_IME_FRENCH_SUPPORT = false";
}
if ($ENV{MTK_IME_GERMAN_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_GERMAN_SUPPORT = true";
}
if ($ENV{MTK_IME_GERMAN_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_GERMAN_SUPPORT = false";
}
if ($ENV{MTK_IME_SPANISH_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_SPANISH_SUPPORT = true";
}
if ($ENV{MTK_IME_SPANISH_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_SPANISH_SUPPORT = false";
}
if ($ENV{MTK_IME_ITALIAN_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_ITALIAN_SUPPORT = true";
}
if ($ENV{MTK_IME_ITALIAN_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_ITALIAN_SUPPORT = false";
}
if ($ENV{MTK_IME_PORTUGUESE_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_PORTUGUESE_SUPPORT = true";
}
if ($ENV{MTK_IME_PORTUGUESE_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_PORTUGUESE_SUPPORT = false";
}
if ($ENV{MTK_IME_TURKISH_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_TURKISH_SUPPORT = true";
}
if ($ENV{MTK_IME_TURKISH_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_TURKISH_SUPPORT = false";
}
if ($ENV{MTK_IME_INDONESIAN_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_INDONESIAN_SUPPORT = true";
}
if ($ENV{MTK_IME_INDONESIAN_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_INDONESIAN_SUPPORT = false";
}
if ($ENV{MTK_IME_MALAY_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_MALAY_SUPPORT = true";
}
if ($ENV{MTK_IME_MALAY_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_MALAY_SUPPORT = false";
}
if ($ENV{MTK_IME_HINDI_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_HINDI_SUPPORT = true";
}
if ($ENV{MTK_IME_HINDI_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_HINDI_SUPPORT = false";
}
if ($ENV{MTK_IME_ARABIC_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_ARABIC_SUPPORT = true";
}
if ($ENV{MTK_IME_ARABIC_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_ARABIC_SUPPORT = false";
}
if ($ENV{MTK_IME_THAI_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_THAI_SUPPORT = true";
}
if ($ENV{MTK_IME_THAI_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_THAI_SUPPORT = false";
}
if ($ENV{MTK_IME_VIETNAM_SUPPORT} eq "yes")
{
	push @ime_opt_arr, "MTK_IME_VIETNAM_SUPPORT = true";
}
if ($ENV{MTK_IME_VIETNAM_SUPPORT} eq "no")
{
	push @ime_opt_arr, "MTK_IME_VIETNAM_SUPPORT = false";
}
#my $MTK_IME_SUPPORT = $ime_opt_arr[1];
#if ($MTK_IME_SUPPORT=~m/\s*(\w+)\s*=\s*(\w+)/) {
#	print OUT_FILE "\tpublic static final boolean $1 = $2;\n";
#}
    print "test1 @ime_opt_arr\n";
for (my $index = 0; $index < @ime_opt_arr; $index++) {
	
    my $word = $ime_opt_arr[$index]; 
    
    if ($word=~m/\s*(\w+)\s*=\s*(\w+)/) {
        my $additional_check = need_additional_check($1);
        if ($additional_check == 0) {
            if ($2 eq "true" || $2 eq "false") {
           		print OUT_FILE "\tpublic static final boolean \t$1 = $2;\n";
    	    } else {
    	        print OUT_FILE "\tpublic static final String  \t$1 = \"$2\";\n";
    	    }
#			print "Not additional check  ", $1, "\n";
        }
        else
        {
#			print "Additional check  ", $1, "\n";
        	if ($2 eq "false") {
               		print OUT_FILE "\tpublic static final boolean \t$1 = $2;\n";
        	}
        	else 
        	{
        		my $state = get_support_state($1, $2);
        		if ($state == 1) {
               			print OUT_FILE "\tpublic static final boolean \t$1 = true;\n";
        		} else {
        			if ($2 eq "true") {
        				print OUT_FILE "\t//Forced to close\n";
        			}
               		print OUT_FILE "\tpublic static final boolean \t$1 = false;\n";
        		}
        	}
        }
    }
}


print OUT_FILE "}\n";
close(OUT_FILE);

# 1: dependency check true; 0: dependency check false; #
sub get_support_state {
	my $opt, $val;
	my $dep;
	($opt, $val) = @_;
	if (defined $ime_dependency{$opt}) {
		$dep = $ime_dependency{$opt};
		if (defined $locales_std{$dep}) {
			return 1;
		}
	}
	return 0;
}

# 1: occured in dependency check table; 0: not occured in dependency check table #
sub need_additional_check {
	my $opt;
	($opt) = @_;
	if (defined $ime_dependency{$opt}) {
    	return 1;
	}
	return 0;
}


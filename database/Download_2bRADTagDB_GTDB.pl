#!/usr/bin/perl
#Author:ZHANG Yufeng, yfz96@connect.hku.hk
use strict;
use warnings;
use File::Basename qw(dirname basename);
use Cwd 'abs_path';

$ARGV[0] ||="2B-RAD-M-ref_db_GTDB";

#if($#ARGV!=0){
#	print STDERR "perl $0 outdir\n";
#	exit 1;
#}

my $outdir=$ARGV[0];#下载目录

$outdir=abs_path($outdir);
&CheckDir("$outdir");


my @a=('abfh_classify');
my @b=('BcgI.species');#需要下载的库文件
my @c=('copy_number_matrix');
my @d=('genome_to_ko');
#my @b=('BcgI.species','CjePI.species');#需要下载的库文件

my %hash_path=(
	'abfh_classify'=>['https://figshare.com/ndownloader/files/45171355/abfh_classify_with_speciename.txt.gz',],

	'BcgI.species' =>['https://figshare.com/ndownloader/files/45172711/BcgI.species.fa.gz0',
	                  'https://figshare.com/ndownloader/files/45172714/BcgI.species.fa.gz1',
	                  'https://figshare.com/ndownloader/files/45172705/BcgI.species.fa.gz2',],
	'copy_number_matrix' =>['https://figshare.com/ndownloader/files/44974513/copy_number_matrix.tar.gz'],
	'genome_to_ko' => ['https://figshare.com/ndownloader/files/44974669/genome_to_ko.tsv.gz']
	);

my %hash_md5=(
	'abfh_classify'=>['6f501521429b7f0fd6ec3a33ffb5b7c8',],
	'BcgI.species' =>['a3cb5394c97d858f17ed4c7a2e256ae5',
	                  'bf167c9e539a072c29fd570d8478c90b',
	                  '956b9b89caf56f65cf12e0cf80ea9be3',],
	'copy_number_matrix' =>['2a77fc70bfe5924cd16441ddddcceabc'],
	'genome_to_ko' =>['cacf3e96c2ed046ff97ae29f5babe993']
	);

#合并后文件md5
my %complete_md5=(
	'BcgI.species' =>'dacf09b22e7900462fa1f7f27316bee1'
	);

#download abfh_classify
for my $i(@a){
	my @tmp=split /\//,$hash_path{$i}[0];
	my $url=join("/",@tmp[0..$#tmp-1]);
	my $name=$tmp[-1];
	my $file_md5;#下载的文件的MD5值
	while(1){
		if(-e "$outdir/$name"){
			chomp($file_md5=`md5sum $outdir/$name`);
			$file_md5=(split /\s+/,$file_md5)[0];
		}
		if(-e "$outdir/$name" && $file_md5 eq $hash_md5{$i}[0]){
			print STDOUT "File $name has been downloaded.\n";
			last;
		}else{
			`wget -t 0 -O $outdir/$name $url`;
		}
	}
}

#下载数据库文件
for my $i(@b){
	my $cat="";
	while(1){
		my $md5;
		if(-e "$outdir/$i.fa.gz"){#存在完成文件
			chomp($md5=`md5sum $outdir/$i.fa.gz`);
			$md5=(split /\s+/,$md5)[0];
		}
		if(-e "$outdir/$i.fa.gz" && $md5 eq $complete_md5{$i}){
			print STDOUT "File $i.fa.gz hash been downloaded.\n";
			`rm -rf $cat`;
			last;
		}else{
			for my $j(0..$#{$hash_path{$i}}){#循环每个文件
				my @tmp=split /\//,$hash_path{$i}[$j];
				my $url=join("/",@tmp[0..$#tmp-1]);
				my $name=$tmp[-1];
				my $file_md5;#下载的文件的MD5值
				while(1){
					if(-e "$outdir/$name"){
						chomp($file_md5=`md5sum $outdir/$name`);
						$file_md5=(split /\s+/,$file_md5)[0];
					}
					if(-e "$outdir/$name" && $file_md5 eq $hash_md5{$i}[$j]){
						print STDOUT "File $name has been downloaded.\n";
						$cat .=" $outdir/$name";
						last;
					}else{
						`wget -t 0 -O $outdir/$name $url`;
					}
				}
			}
			`cat $cat > $outdir/$i.fa.gz`;
		}
	}
}

# Download copy_number_matrix
for my $i(@c){
        my @tmp=split /\//,$hash_path{$i}[0];
        my $url=join("/",@tmp[0..$#tmp-1]);
        my $name=$tmp[-1];
        my $file_md5;#下载的文件的MD5值
        while(1){
                if(-e "$name"){
                        chomp($file_md5=`md5sum $name`);
                        $file_md5=(split /\s+/,$file_md5)[0];
                }
                if(-e "$name" && $file_md5 eq $hash_md5{$i}[0]){
		 	print STDOUT "File $name has been downloaded.\n";
			`tar -xzvf $name`;
			`rm -rf $name`;
                        last;
                }else{
                        `wget -t 0 -O $name $url`;
                }
        }
}

# Download genome_to_ko
for my $i(@d){
        my @tmp=split /\//,$hash_path{$i}[0];
        my $url=join("/",@tmp[0..$#tmp-1]);
        my $name=$tmp[-1];
        my $file_md5;#下载的文件的MD5值
        while(1){
                if(-e "$name"){
                        chomp($file_md5=`md5sum $name`);
                        $file_md5=(split /\s+/,$file_md5)[0];
                }
                if(-e "$name" && $file_md5 eq $hash_md5{$i}[0]){
                        print STDOUT "File $name has been downloaded.\n";
                        `gunzip $name`;
			last;
                }else{
                        `wget -t 0 -O $name $url`;
                }
        }
}

print STDOUT "Congratulations! All databases have been downloaded.\n";

sub CheckDir{
	my $file = shift;
	unless( -d $file ){
		if( -d dirname($file) && -w dirname($file) ){system("mkdir $file");}
		else{print STDERR "$file not exists and cannot be built\n";exit 1;}
	}
	return 1;
}







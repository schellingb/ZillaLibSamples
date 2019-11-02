import os, glob, sys, subprocess, zipfile, shutil, time

#read MSBUILD_PATH, OUT_DIR and ANDROID_* variables for signing APK files from external file 'build-all.cfg' (not checked into version control)
MSBUILD_PATH = 'C:/Program Files (x86)/MSBuild/12.0/Bin/MSBuild.exe'
OUT_DIR = 'Builds'
WEB_GZ = False
exec (file('build-all.cfg').read() if os.path.exists('build-all.cfg') else '')
WEB_GZ = ('.gz' if WEB_GZ else '')

#check if directories for unused assets already exist, abort if so
assert not os.path.exists('Data-Unused'), 'Temporary asset directory "' + 'Data-Unused' + '" still exists, please check (crashed when executed last time?)'

#build list of assets with path names in Data and in Data-Unused
assets = []
for root, dirs, filenames in os.walk('Data'):
	for filename in filenames:
		assets += [[root.replace('\\','/') + '/' + filename,root.replace('Data','Data-Unused',1).replace('\\','/') + '/' + filename]]

# platform specific setup
zl_dir = os.path.realpath(__file__+'/../../ZillaLib').replace('\\', '/')
if sys.platform == 'win32': os.environ['PATH'] += os.pathsep+zl_dir.replace('/', os.sep)+os.sep+'Tools'
linux_cpu_type = 'x86_64' if sys.maxsize > 2**32 else 'x86_32'

#options
is_rebuild = 'rebuild' in sys.argv
select_targets = [k for k in sys.argv if k in ['wasm','emscripten','nacl','android','win32','win64','linux','osx']]
if select_targets == []: select_targets = ['wasm','android','win32','win64','linux','osx']

#create directories for unused assets while building samples that don't need them, and at first move all assets over
for asset in assets:
	if not os.path.exists(os.path.dirname(asset[1])): os.makedirs(os.path.dirname(asset[1]))
	os.rename(asset[0], asset[1])

#create output dir
if not os.path.exists(OUT_DIR): os.makedirs(OUT_DIR)

#loop through all samples
BuildLastRun = 0
for num in range(1, 99):
	try:
		snum = str(num).zfill(2);
		inl = (glob.glob(snum + "-*") or [''])[0]
		if not inl: continue
		inlcode = file(inl).read()
		oneasset = ''
		print '---------------------------------------------------------------------------------------------------------------------------------------------------------------------'
		print '[ASSETS] Building Sample',num,'("' + inl + '"):'
		for asset in assets:
			if (asset[0] in inlcode):
				os.rename(asset[1], asset[0])
				print '    Used Asset:',asset[0]
				oneasset = asset[0]
		if oneasset: os.utime(oneasset, None) #touch asset file so assets get rebuilt

		while BuildLastRun >= int(time.time()):pass #must be at least the next second since last build ended, otherwise make can get confused
		def buildheader(typ):
			print '---------------------------------------------------------------------------------------------------------------------------------------------------------------------'
			print '[' + typ + '] Building Sample',num,'("' + inl + '"):'
		def buildfooter():
			print '---------------------------------------------------------------------------------------------------------------------------------------------------------------------'
			print ''
			sys.stdout.flush()
		def building(pargs):
			print '    ****    Executing:',pargs,'...'
			p = subprocess.Popen(pargs, bufsize=1, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
			while True:
				l = p.stdout.readline()
				if not l: break
				if not l.strip(): continue
				sys.stderr.write('            ' + l.rstrip()[0:180] + "\n")
				sys.stderr.flush()
			pret = p.wait()
			assert pret == 0, '    BUILD RETURNED ERROR, ABORTING'
			global BuildLastRun
			BuildLastRun = int(time.time())
		def buildcopy(src, trg):
			print '    ****    Copying',src,'to',OUT_DIR+'/'+trg,'...'
			shutil.copy2(src, OUT_DIR+'/'+trg)
		def buildzip(trgzip, src, trg):
			print '    ****    Zipping',src,'into',OUT_DIR+'/'+trgzip,'as',trg,'...'
			z = zipfile.ZipFile(OUT_DIR+'/'+trgzip,'w',zipfile.ZIP_DEFLATED);
			z.write(src, trg);[z.write(r+os.sep+f, r.replace(src, trg, 1)+os.sep+f) for r,d,fs in os.walk(src) for f in fs]
			z.close()
		def buildcheck(name, trg):
			if select_targets and name not in select_targets: return False
			return is_rebuild or not os.path.exists(OUT_DIR+'/'+trg) or os.path.getmtime(OUT_DIR+'/'+trg) < os.path.getmtime(inl)

		if sys.platform == 'win32':
			if buildcheck('wasm', 'ZillaLibSample-' + snum + '.js'+WEB_GZ):
				buildheader('WEBASSEMBLY')
				building(['make', '-j', '4', 'wasm-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num), 'W=ZillaLibSampleMain.cpp' + (' ' + oneasset if oneasset else '')])
				buildcopy('Release-wasm/ZillaLibSamples.js'+WEB_GZ, 'ZillaLibSample-' + snum + '.js'+WEB_GZ)
				buildfooter()

			if buildcheck('emscripten', 'ZillaLibSample-' + snum + '.js'+WEB_GZ):
				buildheader('EMSCRIPTEN')
				building(['make', '-j', '4', 'emscripten-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num), 'W=ZillaLibSampleMain.cpp' + (' ' + oneasset if oneasset else '')])
				buildcopy('Release-emscripten/ZillaLibSamples' + ('_WithData' if oneasset else '') + '.js'+WEB_GZ, 'ZillaLibSample-' + snum + '.js'+WEB_GZ)
				buildfooter()

			if buildcheck('nacl', 'ZillaLibSample-' + snum + '.pexe'+WEB_GZ):
				buildheader('NACL')
				building(['make', '-j', '4', 'nacl-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num), 'W=ZillaLibSampleMain.cpp' + (' '+oneasset if oneasset else '')])
				buildcopy('Release-nacl/ZillaLibSamples' + ('_WithData' if oneasset else '') + '.pexe'+WEB_GZ, 'ZillaLibSample-' + snum + '.pexe'+WEB_GZ)
				buildfooter()

			if buildcheck('android', 'ZillaLibSample-' + snum + '.apk'):
				buildheader('ANDROID')
				building(['make', '-j', '4', 'android-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num), 'W=ZillaLibSampleMain.cpp'])
				building(['make', 'android-sign', 'SIGN_OUTAPK='+OUT_DIR+'/ZillaLibSample-' + snum + '.apk', 'SIGN_KEYSTORE='+ANDROID_SIGN_KEYSTORE, 'SIGN_STOREPASS='+ANDROID_SIGN_STOREPASS, 'SIGN_KEYALIAS='+ANDROID_SIGN_KEYALIAS, 'SIGN_KEYPASS='+ANDROID_SIGN_KEYPASS])
				buildfooter()

			if buildcheck('win32', 'ZillaLibSample-' + snum + '_Win32.zip'):
				buildheader('WIN32')
				if os.path.exists('Release-vs2013\ZillaLibSampleMain.obj'): os.remove('Release-vs2013\ZillaLibSampleMain.obj')
				building('"'+MSBUILD_PATH+'" /p:Configuration=Release;Platform=Win32;CmdLinePreprocessorDefinitions="ZILLALIBSAMPLES_NUMBER=' + str(num) + (';ZILLALIBSAMPLES_HASDATA"' if oneasset else '";SkipDataAssets=1') + ' ZillaLibSamples-vs.vcxproj')
				buildzip('ZillaLibSample-' + snum + '_Win32.zip', 'Release-vs2013/ZillaLibSamples' + ('_WithData' if oneasset else '') + '.exe', 'ZillaLibSamples-' + snum + '.exe')
				buildfooter()

			if buildcheck('win64', 'ZillaLibSample-' + snum + '_Win64.zip'):
				buildheader('WIN64')
				if os.path.exists('Release-vs2013x64\ZillaLibSampleMain.obj'): os.remove('Release-vs2013x64\ZillaLibSampleMain.obj')
				building('"'+MSBUILD_PATH+'" /p:Configuration=Release;Platform=x64;CmdLinePreprocessorDefinitions="ZILLALIBSAMPLES_NUMBER=' + str(num) + (';ZILLALIBSAMPLES_HASDATA"' if oneasset else '";SkipDataAssets=1') + ' ZillaLibSamples-vs.vcxproj')
				buildzip('ZillaLibSample-' + snum + '_Win64.zip', 'Release-vs2013x64/ZillaLibSamples' + ('_WithData' if oneasset else '') + '.exe', 'ZillaLibSamples-' + snum + '.exe')
				buildfooter()

		if sys.platform == 'linux2':
			if buildcheck('linux', 'ZillaLibSample-' + snum + '_linux_' + linux_cpu_type + '.zip'):
				buildheader('LINUX')
				building(['make', '-j', '4', 'linux-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num) + (' ZILLALIBSAMPLES_HASDATA' if oneasset else ''), 'W=ZillaLibSampleMain.cpp' + (' ' + oneasset if oneasset else '')])
				buildzip('ZillaLibSample-' + snum + '_linux_' + linux_cpu_type + '.zip', 'Release-linux/ZillaLibSamples_' + linux_cpu_type + ('_WithData' if oneasset else ''), 'ZillaLibSample-' + snum)
				buildfooter()

		if sys.platform == 'darwin':
			if buildcheck('osx', 'ZillaLibSample-' + snum + '_osx.zip'):
				buildheader('OSX')
				building(['make', '-j', '4', 'osx-release', 'D=ZILLALIBSAMPLES_NUMBER=' + str(num) + (' ZILLALIBSAMPLES_HASDATA' if oneasset else '')])
				buildzip('ZillaLibSample-' + snum + '_osx.zip', 'ZillaLibSamples-OSX.xcodeproj/Release/ZillaLibSamples.app', 'ZillaLibSample-' + snum + '.app')
				buildfooter()

	except: import traceback; traceback.print_exception(sys.exc_info()[0], sys.exc_info()[1], sys.exc_info()[2]); break;
	finally:
		#move all assets back to unused for building the next sample
		for asset in assets:
			if os.path.exists(asset[0]): os.rename(asset[0], asset[1])

#removing temporary directories
for asset in assets:
	os.rename(asset[1], asset[0])
	try: os.rmdir(os.path.dirname(asset[1]))
	except: pass

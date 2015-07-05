# timelagfilter

Open-source VST3 (x86/x64) plugin that uses phase effects to mangle audio in the time domain

## Features:
- New, original core audio effect that delays groups of frequencies in time
- Smooth variation between subtle aura and smeared, barely recognizable soundscapes
- Traditional phasors and stereo separation are special cases
- Main parameters have preset graphs with modulatable values, or draw the graph manually
- Real-time-updating color graphs of plugin phase-domain behavior
- Input and output mixers with negative phase options on all controls
- Full VST3 automation support: modulation, touch recording, custom displayed values

## To download:
In the bin folder, click on TimeLagFilter.dll if you have a 64-bit machine or TimeLagFilter32.dll if you are running 32-bit. On the following screen, click the Raw button to download the file, and save it in your DAW's VST plugins folder. Restart your DAW (or tell it to scan for new plugins) and it will detect the plugin.

## Plugin format:
If you're using a 64-bit computer, even if your DAW for some reason isn't 64-bit (but can support 64-bit plugins in bridged mode), please use the 64-bit version. The 32-bit version was tested and worked correctly on a 64-bit computer, but it used about 50-100% more CPU due to the added virtualization layer. If you're using a 32-bit computer, the 64-bit version won't work!

This is a VST3 plugin, and supports many of the host integration features of the VST3 standard. If for some reason your DAW only supports VST2, contact me and I'll take a look at releasing a VST2 version--but key features (such as automation) may be cut.

I'd make an AU version, but I don't have a Mac to compile and test it on! If anyone is interested in helping out with this, please let me know.

## Legal:
timelagfilter: Copyright (C) 2015 Sauraen, sauraen@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

using System;
using NetCoreAudio;
using Cs;

namespace Cs 
{
    class SoundSystem {
        public static void PlaySound(string filepath, bool wait = false) {
            string fullpath = Path.GetFullPath(filepath);
            Player player = new Player();
            player.Play(filepath);
            TimeSpan shorttime = new TimeSpan(1);
            while (wait && player.Playing) Thread.Sleep(shorttime);
        }   
    }
}
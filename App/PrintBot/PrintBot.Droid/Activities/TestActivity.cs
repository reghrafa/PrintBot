using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using PrintBot.Infrastructure.ViewModels;
using Android.Content.Res;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "TestActivity")]
    public class TestActivity : Activity
    {
        private BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.TestActivityLayout);

            FindViewById<Button>(Resource.Id.test_epic_shit).Click += TestActivity_Click;
            
            // Create your application here
        }

        private async void TestActivity_Click(object sender, EventArgs e)
        {
            // Read Hex
            string hexFile;
            AssetManager assets = Assets;
            using (StreamReader sr = new StreamReader(assets.Open("samplehex.txt")))
            {
                hexFile = sr.ReadToEnd();
            }
            var split = Regex.Split(hexFile, @"(?=:)");

            _vm.StartListening();

            // Reset processor
            await _vm.WriteAsync(Encoding.ASCII.GetBytes("A"));

            //TODO: Error handling
            var timeout = 2000;
            var delay = 50;
            for (int i = 0; i < timeout/ delay; i++)
            {
                if(_vm.ListeningResult.Contains("start real bootloader"))
                {
                    //_vm.ListeningResult = "";
                    break;
                }
                await Task.Delay(delay);
            }

            // Activate Programming Mode
            await _vm.WriteAsync(Encoding.ASCII.GetBytes("p"));
            for (int i = 0; i < timeout / delay; i++)
            {
                if (_vm.ListeningResult.Contains("paste the hex file"))
                {
                    //_vm.ListeningResult = "";
                    break;
                }
                await Task.Delay(delay);
            }

            // Send HexFile
            var delayCtr = 0;
            string lines = "";
            foreach (var line in split)
            {
                if (string.IsNullOrWhiteSpace(line)) continue;

                lines += line.Replace("\r\n", string.Empty);


            }
            int buffersize = 8;
            byte[] buffer = new byte[buffersize];
            var bts = Encoding.ASCII.GetBytes(lines);

            int offset = 0;
            foreach (var b in bts)
            {
                buffer[offset] = b;
                ++offset;
                if (offset == buffersize)
                {
                    await _vm.WriteAsync(buffer);
                    await Task.Delay(150);
                    buffer = new byte[buffersize];
                    offset = 0;
                }


            }
            await _vm.WriteAsync(buffer);


            //Done.
        }
    }
}
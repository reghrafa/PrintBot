using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using PrintBot.Infrastructure.ViewModels;

namespace PrintBot.Android.Fragments
{
    public class BluetoothSendFragment : Fragment
    {
        private BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;

        Button btnSend;

        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            // _vm.PropertyChanged += _vm_PropertyChanged;
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.bluetooth_send_layout, container, false);

            btnSend = view.FindViewById<Button>(Resource.Id.bluetooth_send_SendButton);
            btnSend.Click += delegate { SendStuff(); };

            return view;
        }

        private async void SendStuff()
        {
            await _vm.WriteAsync(new byte[] { 1 });
        }

        private void _vm_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {

        }
    }
}
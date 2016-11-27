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
using PrintBot.Droid.Adapter;
using Plugin.BLE.Abstractions.Contracts;
using Android;

namespace PrintBot.Droid.Fragments
{
    public class BluetoothFragment : Fragment
    {
        private BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;

        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            _vm.PropertyChanged += _vm_PropertyChanged;
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.bluetooth_layout, container, false);
            ChangeFragment();
            return view;
        }

        private void _vm_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {

            if (e.PropertyName == nameof(BluetoothViewModel.Connected))
            {
                ChangeFragment();
            }
        }

        private void ChangeFragment()
        {
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            if (_vm.Connected)
            {
                ft.Replace(Resource.Id.bluetooth_fragment_container, new BluetoothScanFragment());
            }
            else
            {
                ft.Replace(Resource.Id.bluetooth_fragment_container, new BluetoothSendFragment());
            }
        }
    }
}
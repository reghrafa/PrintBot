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
    public class BluetoothScanFragment : Fragment
    {
        private BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;

        Button btnScan;
        ListView listViewKnownDevices;
        ListView listViewFoundDevices;
        TextView textKnownDevices;
        TextView textFoundDevices;
        ProgressBar progressBar;
        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Create your fragment here
            _vm.PropertyChanged += _vm_PropertyChanged;
        }

        private void _vm_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(BluetoothViewModel.ScanStatus))
            {
                progressBar.Visibility = _vm.ScanStatus ? ViewStates.Visible : ViewStates.Gone;
            }
            if (e.PropertyName == "FoundDevices")
            {
                textFoundDevices.Visibility = _vm.FoundDevices.Count > 0 ? ViewStates.Visible : ViewStates.Gone;
            }
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.bluetooth_scan_layout, container, false);

            // UI Controls
            btnScan = view.FindViewById<Button>(Resource.Id.bluetooth_ScanButton);
            listViewFoundDevices = view.FindViewById<ListView>(Resource.Id.bluetooth_ListViewFoundDevices);
            listViewKnownDevices = view.FindViewById<ListView>(Resource.Id.bluetooth_ListViewKnownDevices);
            progressBar = view.FindViewById<ProgressBar>(Resource.Id.bluetooth_ProgressBar);
            textFoundDevices = view.FindViewById<TextView>(Resource.Id.Bluetooth_FoundDevicesTextView);
            textKnownDevices = view.FindViewById<TextView>(Resource.Id.Bluetooth_KnownDevicesTextView);

            progressBar.Visibility = ViewStates.Gone;
            textFoundDevices.Visibility = ViewStates.Gone;
            textKnownDevices.Visibility = ViewStates.Gone;

            _vm.GetPairedOrKnownDevices();
            if (_vm.PairedDevices.Count > 0)
            {
                textKnownDevices.Visibility = ViewStates.Visible;
            }

            // Events
            btnScan.Click += delegate { _vm.StartScanningForDevicesAsync(); };
            listViewFoundDevices.ItemClick += ListViewFoundDevices_ItemClick;
            listViewKnownDevices.ItemClick += ListViewKnownDevices_ItemClick;

            // ListViews
            listViewFoundDevices.Adapter = new BluetoothListAdapter(this.Activity, _vm.FoundDevices);
            listViewKnownDevices.Adapter = new BluetoothListAdapter(this.Activity, _vm.PairedDevices);

            return view;
        }

        private async void ListViewKnownDevices_ItemClick(object sender, AdapterView.ItemClickEventArgs e)
        {
            _vm.StopScanningForDevicesAsync();
            var device = _vm.PairedDevices[e.Position];
            await _vm.ConnectToKnownDeviceAsync(device.Id);
        }

        private async void ListViewFoundDevices_ItemClick(object sender, AdapterView.ItemClickEventArgs e)
        {
            _vm.StopScanningForDevicesAsync();
            var device = _vm.FoundDevices[e.Position];
            await _vm.ConnectToDeviceAsync(device);
        }
    }
}
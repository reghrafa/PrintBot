using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.Graphics;
using Android.Graphics.Drawables;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using PrintBot.Droid.Fragments;
using PrintBot.Infrastructure.ViewModels;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Bluetooth Setup")]
    public class BluetoothActivity : PortraitActivity
    {
        BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.bluetooth_layout);
            ChangeFragment();
            _vm.PropertyChanged += _vm_PropertyChanged;
            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);
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
            if (!_vm.Connected)
            {
                ft.Replace(Resource.Id.bluetooth_fragment_container, new BluetoothScanFragment());
            }
            else
            {
                ft.Replace(Resource.Id.bluetooth_fragment_container, new BluetoothSendFragment());
            }
            ft.Commit();
        }
    }

}
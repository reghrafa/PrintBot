using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using PrintBot.Domain.RequiredInterfaces;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Api.Clients
{
    public class BluetoothClient : IBluetoothClient
    {
        private IAdapter _adapter;
        private IBluetoothLE _bluetooth = CrossBluetoothLE.Current;
        private IService _service;
        private ICharacteristic _characteristic;
        public event DeviceDiscoveredEventHandler DeviceDiscovered;
        public event DeviceConnectedEventHandler DeviceConnected;
        public event DeviceConnectionLostEventHandler DeviceConnectionLost;
        public event DeviceDisconnectedEventHandler DeviceDisconnected;
        public event ScanTimeoutElapsedEventHandler ScanTimeoutElapsed;

        public bool IsScanning { get { return _adapter.IsScanning; } }
        private int _scanTimeout;
        public int ScanTimeout
        {
            get { return _scanTimeout; }
            set { if (_scanTimeout != value) _scanTimeout = value; }
        }

        private int _connectedDevicePosition = 0;
        private int _servicePosition = 0;

        public BluetoothClient()
        {
            _adapter = _bluetooth.Adapter;
            _adapter.DeviceDiscovered += _adapter_DeviceDiscovered;
            _adapter.DeviceConnected += _adapter_DeviceConnected;
            _adapter.DeviceConnectionLost += _adapter_DeviceConnectionLost;
            _adapter.DeviceDisconnected += _adapter_DeviceDisconnected;
            _adapter.ScanTimeoutElapsed += _adapter_ScanTimeoutElapsed;
        }

        #region Adapter events
        private void _adapter_DeviceDisconnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            DeviceDisconnected?.Invoke(e.Device);
        }

        private void _adapter_DeviceConnectionLost(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceErrorEventArgs e)
        {
            DeviceConnectionLost?.Invoke(e.Device, e.ErrorMessage);
        }

        private void _adapter_DeviceConnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            DeviceConnected?.Invoke(e.Device);
        }

        private void _adapter_DeviceDiscovered(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            DeviceDiscovered?.Invoke(e.Device);
        }

        private void _adapter_ScanTimeoutElapsed(object sender, EventArgs e)
        {
            ScanTimeoutElapsed?.Invoke();
        }
        #endregion

        #region Adapter methods
        public async Task StartScanForDevicesAsync()
        {
            await _adapter.StartScanningForDevicesAsync();
        }

        public async Task StopScanForDevices()
        {
            await _adapter.StopScanningForDevicesAsync();
        }

        public ObservableCollection<IDevice> GetPairedDevices()
        {
            return new ObservableCollection<IDevice>(_adapter.GetSystemConnectedOrPairedDevices());
        }

        public async Task ConnectToDeviceAsync(IDevice device)
        {
            await _adapter.ConnectToDeviceAsync(device);
        }

        public async Task ConnectToKnownDeviceAsync(Guid id)
        {
            await _adapter.ConnectToKnownDeviceAsync(id);
        }

        public async Task DisconnectDeviceAsync(IDevice device)
        {
            await _adapter.DisconnectDeviceAsync(device);
        }

        bool IBluetoothClient.IsScanning()
        {
            return IsScanning;
        }

        public void SetScanTimeout(int ms)
        {
            ScanTimeout = ms;
        }

        public int GetScanTimeout()
        {
            return ScanTimeout;
        }

        public async Task<IList<IService>> UpdateServicesAsync(int position)
        {
            _connectedDevicePosition = position;
            return await _adapter.ConnectedDevices[_connectedDevicePosition].GetServicesAsync();
        }

        public async Task<IEnumerable<ICharacteristic>> UpdateCharacteristicsAsync(int positionInServices)
        {
            _servicePosition = positionInServices;
            var services = await _adapter.ConnectedDevices[_connectedDevicePosition].GetServicesAsync();
            _service = services[_servicePosition];
            return await services[_servicePosition].GetCharacteristicsAsync();
        }

        // ToDo: Event handling
        public async Task<byte[]> ReadAsync()
        {
            if (_characteristic == null)
            {
                var tmp = await _service.GetCharacteristicsAsync();
                _characteristic = tmp.First();
            }
            return await _characteristic.ReadAsync();
        }
        #endregion
    }
}

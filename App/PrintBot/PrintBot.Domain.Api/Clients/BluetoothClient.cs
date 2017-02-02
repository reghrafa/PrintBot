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

        public ICharacteristic GetCharacteristic()
        {
            return _characteristic ?? null;
        }
        public bool IsScanning { get { return _adapter.IsScanning; } }
        private int _scanTimeout;
        public int ScanTimeout
        {
            get { return _scanTimeout; }
            set { if (_scanTimeout != value) _scanTimeout = value; }
        }

        private int _indexOfConnectedDevice = 0;

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

        public async Task StopScanForDevicesAsync()
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
            _indexOfConnectedDevice = GetIndexOfConnectedDevice(device);
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

        /// <summary>
        /// Search for the service and store it in _service if it was found.
        /// </summary>
        /// <param name="name">Name of the service</param>
        /// <returns></returns>
        public async Task<bool> SetServiceByNameAsync(string name)
        {
            var listOfServices = await _adapter.ConnectedDevices[_indexOfConnectedDevice].GetServicesAsync();
            foreach (IService service in listOfServices)
            {
                if (service.Name.Equals(name))
                {
                    _service = service;
                    return true;
                }
            }
            _service = null;
            return false;
        }

        /// <summary>
        /// Search for the characteristic of _service and store it in _characteristic if it was found.
        /// </summary>
        /// <param name="name">Name of the service</param>
        /// <returns></returns>
        public async Task<bool> SetCharacteristicByNameAsync(string name)
        {
            if (_service != null)
            {
                var listOfCharacteristics = await _service.GetCharacteristicsAsync();
                foreach (ICharacteristic characteristic in listOfCharacteristics)
                {
                    if (characteristic.Name.Equals(name))
                    {
                        _characteristic = characteristic;
                        return true;
                    }
                }
            }
            _characteristic = null;
            return false;
        }

        // ToDo: Event handling
        public async Task<byte[]> ReadAsync()
        {
            if (_characteristic == null) return null;
            return await _characteristic.ReadAsync();
        }
        public async Task WriteAsync(byte[] data)
        {
            if (_characteristic == null) return;
            await _characteristic.WriteAsync(data);
        }
        #endregion

        #region private methods
        private int GetIndexOfConnectedDevice(IDevice device)
        {
            return _adapter.ConnectedDevices.IndexOf(device);
        }

        public async Task<bool> IsValidDeviceAsync(string serviceName, string characteristicName, IDevice device)
        {
            var listOfServices = await device.GetServicesAsync();
            foreach (IService service in listOfServices)
            {
                if (service.Name.Equals(serviceName))
                {
                    var listOfCharacteristics = await service.GetCharacteristicsAsync();
                    foreach (ICharacteristic characteristic in listOfCharacteristics)
                    {
                        if (characteristic.Name.Equals(characteristicName))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        #endregion
    }
}

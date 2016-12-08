using System;

namespace PrintBot.Domain.Models
{
    public class BordEditor_PysicalPin
    {
        private string _pinType;
        private int _pinNr;

        public string PinType
        {
            get
            {
                return _pinType;
            }

            set
            {
                _pinType = value;
            }
        }

        public int PinNr
        {
            get
            {
                return _pinNr;
            }

            set
            {
                _pinNr = value;
            }
        }

        public BordEditor_PysicalPin(string name, int nr)
        {
            this._pinType = name;
            this._pinNr = nr;
        }
    }
}
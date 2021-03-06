﻿using NetCon.model;
using NetCon.util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NetCon.repo
{

    public class CaptureState
    {
        public class CaptureOn: CaptureState { }
        public class CaptureOff: CaptureState { }
        public class CaptureError: CaptureState {
            private Exception error;
            public Exception Error { get { return error; } }
            public CaptureError(Exception err)
            {
                error = err;
            }
        }
    }

    interface IFrameRepository<T>
    {

        void applyFilters(FiltersConfiguration<T> config);
        void startCapture();
        void stopCapture();

        void resumeCapture();
        void pauseCapture();

        Subject<T> FrameSubject { get; }
        Subject<CaptureState> CaptureState { get; }
    }
}

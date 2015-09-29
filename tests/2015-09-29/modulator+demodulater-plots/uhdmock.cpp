#include "uhdmock.h"

UhdMock::UhdMock(StreamFunction * func, double rate, double fc, double gain, size_t frameSize) :
    Streamer(),
    _maxBuffSize(16384),
    _buffer(new SharedBuffer<std::complex<double> >()),
    _func(func),
    _funcMutex(),
    _streamThread(),
    _isStreaming(false),
    _rate(rate),
    _fc(fc),
    _gain(gain),
    _frameSize(frameSize),
    _timeVec(new SharedQVector<double>()),
    _ampVec(new SharedQVector<double>())
{

}

void UhdMock::setMaxBuffer(size_t maxBuffSize)
{
    _maxBuffSize = maxBuffSize;
}

void UhdMock::startStream()
{
    _isStreaming = true;
    _streamThread = boost::thread(&UhdMock::runStream, this);
}

void UhdMock::changeFunc(StreamFunction * func)
{
    boost::unique_lock < boost::mutex > funcLock(_funcMutex);
    _func.reset(func);
}

void UhdMock::stopStream()
{
    _isStreaming = false;
    _streamThread.join();
}

void UhdMock::runStream()
{
    double period = 1/_rate;
    double t = 0.0;
    std::complex<double> dataPoint{0.0, 0.0};

    while(_isStreaming)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds((long)(period * 1e6 * _frameSize)));

        // Get unique access.
        boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
        boost::unique_lock < boost::shared_mutex > lock (*mutex.get());
        boost::unique_lock < boost::mutex > funcLock(_funcMutex);

        boost::shared_ptr < boost::shared_mutex > timeMutex = _timeVec->getMutex();
        boost::unique_lock < boost::shared_mutex > timeLock (*timeMutex.get());
        boost::shared_ptr < boost::shared_mutex > ampMutex = _ampVec->getMutex();
        boost::unique_lock < boost::shared_mutex > ampLock (*ampMutex.get());

        // Generate a frame of data.
        for(unsigned int n = 0; n < _frameSize; ++n)
        {
            dataPoint = _func->calc(t) * _gain;

            _buffer->getBuffer().push_back(dataPoint);

            // Prohibit data buffer from getting too large.
            if(_buffer->getBuffer().size() > _maxBuffSize)
            {
                _buffer->getBuffer().pop_front();
            }

            // Add Data to the vector.
            _timeVec->getData().push_back(t);
            _ampVec->getData().push_back(dataPoint.real());
            if(_timeVec->getData().size() > 2048)
            {
                _timeVec->getData().pop_front();
            }
            if(_ampVec->getData().size() > 2048)
            {
                _ampVec->getData().pop_front();
            }

            t += period;
        }
    }
    std::cout << std::endl << "Closing UHD mock thread" << std::endl;
}

boost::shared_ptr < SharedBuffer<std::complex<double> > > UhdMock::getBuffer()
{
    return _buffer;
}

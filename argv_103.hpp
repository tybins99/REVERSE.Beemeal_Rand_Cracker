#define argv_105 0x0500

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctype.h>
#include <stack>
#include <string>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <list>
#include <Winbase.h>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <sstream>

using namespace std;

class argv_107
{
protected:
	LARGE_INTEGER argv_132;
	LARGE_INTEGER argv_133;

	LONGLONG argv_135;
	LONGLONG argv_134;

public:
	argv_107(void);

	void argv_139(void);
	void argv_140(void);
	double argv_124(void) const;
	string argv_141 (const char *);
};

inline argv_107::argv_107(void) {
	LARGE_INTEGER liFrequency;

	QueryPerformanceFrequency(&liFrequency);
	argv_135 = liFrequency.QuadPart;

	// Calibration
	argv_139();
	argv_140();

	argv_134 = argv_133.QuadPart-argv_132.QuadPart;
}

inline void argv_107::argv_139(void) {
	// Ensure we will not be interrupted by any other thread for a while
//	Sleep(0);
	QueryPerformanceCounter(&argv_132);
}

inline void argv_107::argv_140(void) {
	QueryPerformanceCounter(&argv_133);
}

inline double argv_107::argv_124(void) const {
	return (double)(argv_133.QuadPart-argv_132.QuadPart-argv_134)*1000000.0 / argv_135;
}

inline string argv_107::argv_141 (const char * _msg) {
	argv_140 ();
	double elapsed = argv_124 ();
	char tmp[MAX_PATH];
	if (elapsed < 1000000.0) {
		snprintf (tmp, 1024, "%s : %lf usec", _msg, elapsed);
	}
	else if ((elapsed >= 1000000.0) && (elapsed < 60000000.0)) {
		elapsed = elapsed / 1000000.0;
		snprintf (tmp, 1024, "%s : %lf seconds", _msg, elapsed);
	}
	else {
		elapsed = elapsed / 60000000;
		snprintf (tmp, 1024, "%s : %lf minutes", _msg, elapsed);
	}
	tmp[MAX_PATH - 1] = '\0';
	return tmp;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ DESCRIPTION:
// @@ This object is a thread safe, but not event driven
// @@ simple queue (use argv_110 if you want to have an
// @@ event driven queue !).
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
class argv_111
{
private:
	CRITICAL_SECTION argv_109;
	list<T>	argv_131;

public:
	argv_111 ();
	~argv_111 ();
	void argv_137 (const T&);
	bool get (T&);
	void clear ();
	uint32_t argv_125 (); 
};

template <class T>
argv_111<T>::argv_111 () {
	InitializeCriticalSection(&argv_109);	
}

template <class T>
void argv_111<T>::argv_137 (const T& _elem) {
	EnterCriticalSection (&argv_109);
		argv_131.push_back (_elem);
	LeaveCriticalSection(&argv_109);
}

template <class T>
void argv_111<T>::clear () {
	EnterCriticalSection (&argv_109);
		argv_131.clear ();
	LeaveCriticalSection(&argv_109);
}

template <class T>
uint32_t argv_111<T>::argv_125 () {
	uint32_t size = 0;
	EnterCriticalSection (&argv_109);
		size = argv_131.size ();
	LeaveCriticalSection(&argv_109);
	return (size);
}

template <class T>
bool argv_111<T>::get (T& _elem) {
	bool bret = true;
	EnterCriticalSection (&argv_109);
		if (argv_131.size() > 0) {
			_elem = argv_131.front ();
			argv_131.pop_front ();
		}
		else {
			bret = false;
		}
	LeaveCriticalSection(&argv_109);
	return (bret);
}

template <class T>
argv_111<T>::~argv_111 () {
	DeleteCriticalSection(&argv_109);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ DESCRIPTION:
// @@ This object is a thread safe, event-driven FIFO queue
// @@ that permits to have several consumer and several 
// @@ producers at the same time without corruption of the data 
// @@ structure.
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
class argv_110
{
private:
	bool	argv_113;
	HANDLE	argv_136;
	HANDLE	argv_118;
	HANDLE	argv_119;

	bool	init_done;
	uint32_t	argv_122;
	T *		argv_142;
	uint32_t	first;
	uint32_t	argv_130;
	bool	argv_117 (const T&);
	bool	argv_116 (T&);
	bool	argv_129 ();
	bool	argv_128 ();
	void	argv_114 (HWND);

public:
	argv_110 ();
	bool	init (uint32_t);
	bool	get (T&);
	bool	argv_137 (const T&);
	void	argv_138 ();
	void	argv_112 ();
	~argv_110();
};



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_110
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE: none
// @@
// @@ DESCRIPTION:
// @@ Normal constructor of the argv_110 object.
// @@ It merely set the data to default values.
// @@
// @@ CONTRACT: none
// @@ 
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
argv_110<T>::argv_110 () {
	init_done	= false;
	argv_142			= NULL;
	argv_113	= true;
	argv_122	= 0;
	argv_138 ();
}

template <class T>
void argv_110<T>::argv_114 (HWND) {
	char tmp[256];
	char tmp2[256];
	tmp[0] = '\0';
	uint32_t current = first;
	string total = "";
	strcat (tmp, "[");
	for (;;) {
		if (current == argv_130) {
			break;
		}
		snprintf (tmp2, 256, "%u ", argv_142[current]);
		strcat (tmp, tmp2);
		current = (current + 1) % argv_122;
	}
	strcat (tmp, "]");
	total = tmp;
	snprintf (tmp2, 256, " - first=%u, argv_130=%u", first, argv_130);
	total += tmp2;
	MessageBox (NULL, total.c_str(), "", MB_OK);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: ~argv_110
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE: none
// @@
// @@ DESCRIPTION:
// @@ destructor of the argv_110 object.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
argv_110<T>::~argv_110 () {
	argv_112 ();
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_112
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE: none
// @@
// @@ DESCRIPTION:
// @@ This function destructs safely the memory allocated for the
// @@ argv_110 object.
// @@ HOWEVER all access to the object should have been terminated so
// @@ that a unique thread can call the argv_112 function.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
void argv_110<T>::argv_112 () {
	if (argv_113 == false) {
		WaitForSingleObject (argv_136, INFINITE); // lock the argv_136
			argv_113 = true;
			argv_138 ();
			if (argv_142 != NULL) {
				delete [] argv_142;
				argv_142 = NULL;
			}
			argv_122 = 0;
			CloseHandle (argv_118);
			CloseHandle (argv_119);
		ReleaseMutex (argv_136);
		CloseHandle (argv_136);
	}
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_138
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE: none
// @@
// @@ DESCRIPTION:
// @@ This function just permits to reset the pointers so that
// @@ the queue thinks it is empty.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
void argv_110<T>::argv_138 () {
	first		= 0;
	argv_130		= 0;
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_128
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool: true if the queue is empty.
// @@       false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function is a simple test to check whether the queue
// @@ contains one or more elements or not.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::argv_128 () {
	return (argv_130 == first);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_129
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool: true if the queue is full.
// @@       false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function permits to test whether the queue
// @@ is full of elements or not.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::argv_129 () {
	return (((argv_130 + 1) % argv_122) == first);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_116
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: none
// @@
// @@ IO: 
// @@ _data: T: the retrieve piece of data. 
// @@
// @@ RETURN VALUE:
// @@ bool : true if success, 
// @@        false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function permit to extract next piece of data from
// @@ the queue.
// @@
// @@ CONTRACT: 
// @@ - queue should not be empty prior to this call
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::argv_116 (T& _data) {
	bool bret = true;
	if (argv_128 () == true) {
		bret = false; // illegal to extract when queue is empty
	}
	else {
		_data = argv_142[first];
		first = ((first + 1) % argv_122);
	}
	return (bret);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_117
// @@
// @@ INPUT:
// @@ _data: T: the data to insert into the queue.
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool : true if success, 
// @@        false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function permits to perform the extraction of the next
// @@ element from the queue in a FIFO way.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::argv_117 (const T& _data) {
	bool bret = true;
	if (argv_129 () == true) {
		bret = false; // Error - Q is full 
	}
	else {
		argv_142[argv_130] = _data;
		argv_130 = ((argv_130 + 1) % argv_122);
	}
	return (bret);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: init
// @@
// @@ INPUT:
// @@ _full_size: uint32_t: the maximum size of the queue at any time
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool: true if success.
// @@       false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function allocates enough memory to store MAX
// @@ elements into the queue object. MAX being given as
// @@ a parameter.
// @@
// @@ CONTRACT:
// @@ - init should not have been called prior to this call.
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::init (uint32_t _full_size) {
	bool bret = true;
	if (init_done == false) {
		// argv_122 = _full_size;	
		argv_122 = _full_size + 1;	
		argv_142 = new (std::nothrow) T[argv_122];

		if (argv_142 != NULL) {
			argv_136			= CreateMutex (NULL, FALSE, NULL);
			argv_118 = CreateEvent (NULL, TRUE, FALSE, NULL);
			argv_119	= CreateEvent (NULL, TRUE, FALSE, NULL);
			argv_113		= false;
			init_done		= true;
		}
		else { // memory allocation failed
			bret = false;
		}
	}
	return (bret);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: get
// @@
// @@ INPUT: none
// @@
// @@ OUTPUT: 
// @@ _data: T: the retrieved data.
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool: true if success.
// @@       false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function permits to extract the next piece of data
// @@ from the FIFO queue (if any). If no data is present, then
// @@ the process is argv_137 to sleep.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::get (T& _data) {
	bool bret = true;
	WaitForSingleObject (argv_136, INFINITE); // acquire argv_136
	while (argv_128 () == true) {
		SignalObjectAndWait (argv_136, argv_118, INFINITE, FALSE);
		WaitForSingleObject (argv_136, INFINITE);
	}
	bret = argv_116 (_data);
	PulseEvent (argv_119);
	ReleaseMutex (argv_136);
	return (bret);
}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@
// @@ CLASS: argv_110
// @@
// @@ FUNCTION: argv_137
// @@
// @@ INPUT:
// @@ _data: T: the data to be saved to the queue.
// @@
// @@ OUTPUT: none
// @@
// @@ IO: none
// @@
// @@ RETURN VALUE:
// @@ bool: true if success.
// @@       false otherwise.
// @@
// @@ DESCRIPTION:
// @@ This function permits to add a new piece of data to the
// @@ queue. If there is no room, then the process is argv_137 to sleep
// @@ until it is woken up again when room is created.
// @@
// @@ CONTRACT: none
// @@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
template <class T>
bool argv_110<T>::argv_137 (const T& _data) {
	bool bret = true;
	WaitForSingleObject (argv_136, INFINITE); // acquire argv_136
	while (argv_129 () == true) {
		SignalObjectAndWait (argv_136, argv_119, INFINITE, FALSE);
		WaitForSingleObject (argv_136, INFINITE);
	}
	bret = argv_117 (_data);
	PulseEvent (argv_118);
	ReleaseMutex (argv_136);
	return (bret);
}


class argv_108
{
public:
	bool			argv_120;
	uint32_t			argv_121;
	uint32_t			argv_127;
	vector<uint32_t>	argv_144;

	uint64_t			argv_139;
	uint64_t			argv_140;

	argv_108 () {
		argv_120 = false;
	}
};
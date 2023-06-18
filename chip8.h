class chip8 {
public:
	unsigned char  gfx[64 * 32]; 	// Total amount of pixels: 2048
	unsigned char  key[16];
	bool           drawFlag;

private:
	unsigned short pc;				// Program counter
	unsigned short opcode;			// Current opcode
	unsigned short I;				// Index register
	unsigned short sp;				// Stack pointer
	
	unsigned char  V[16];			// V-regs (V0-VF)
	unsigned char  memory[4096];	// Memory (size = 4k)
	unsigned short stack[16];		// Stack (16 levels)

	unsigned char  delay_timer;		// Delay timer
	unsigned char  sound_timer;		// Sound timer

public:
	chip8();
	~chip8();

public:
	void emulateCycle();
	void debugRender();
	bool loadApplication(const char* filename);

private:
	void init();
};

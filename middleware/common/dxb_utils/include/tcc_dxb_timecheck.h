#include <time.h>

#ifdef __cplusplus
extern    "C"
{
#endif

#define USETIMECHECK 0

typedef struct tRecord {
	struct tRecord *nextRecord;
	int recordTime;
}TRECORD;

typedef struct tField {
	char *fieldName;
	struct tField *nextField;
	struct tRecord *recordHeader;
	struct tRecord *recordTail;
	int startTime;
	int stopTime;
	int maxTime;
	int minTime;
	int avgTime;
	int recordCount;
}TFIELD;

typedef struct tTable {
	char *tableName;
	struct tTable *nextTable;
	struct tField *fieldHeader;
	int fieldCount;
}TTABLE;

enum {
	TIMECHECK_START,
	TIMECHECK_STOP,
};

static int calculateTime(struct timespec time);

static void tcc_dxb_timecheck_lock();
static void tcc_dxb_timecheck_unlock();
static void tcc_dxb_timecheck_record_init(TRECORD **record);
static void tcc_dxb_timecheck_field_init(TFIELD **field);
static void tcc_dxb_timecheck_table_init(TTABLE **table);

static void tcc_dxb_timecheck_free(void *ptr);

static void tcc_dxb_timecheck_init();
static void tcc_dxb_timecheck_deinit();

extern void tcc_dxb_timecheck_set(const char *tableName, const char *fieldName, unsigned char ssFlag);

extern void tcc_dxb_timecheck_printlogAVG();
extern void tcc_dxb_timecheck_printlog();
extern void tcc_dxb_timecheck_printfile();

#ifdef __cplusplus
}
#endif

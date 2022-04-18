#include <stdio.h>
#include <malloc.h>

typedef struct p_node{
	int size;
	int block_num;
	int serial_num;
	struct p_node *prev;
	struct p_node *next;
}pBlock;

pBlock *pHead = NULL;
int count_pBlock = 1;

typedef struct m_node{
	int size;
	int serial_num;
	struct m_node *prev;
	struct m_node *next;
}mBlock;

mBlock *mHead = NULL;
int count_mBlock = 1;

void create_mBlock(mBlock *mem_block, int num)
{	
	if(mHead != NULL)
	{
		while(mem_block->next != NULL)
		{
			mem_block = mem_block->next;
		}
	}
	
	int i;	
	for(i=0; i<num; i++)
	{
		mBlock *memory;
		memory = (mBlock*)malloc(sizeof(mBlock));
		
		printf("Enter size of memory block %d: ",count_mBlock);
		scanf("%d",&memory->size);
		memory->serial_num = count_mBlock;
		
		if(mHead == NULL)
		{
			memory->next=NULL;
			memory->prev=NULL;
			mHead = memory;
		}
		else
		{
			memory->next = NULL;
			memory->prev = mem_block;
			
			if(mHead->next == NULL)
			{
				mHead->next = memory;
			}
			else
			{
				mem_block->next = memory;
			}
		}
		
		mem_block = memory;
		count_mBlock++;
	}
	mem_block->next = NULL;
}

void startProcess(mBlock *mem_block, pBlock *proc_block, int num)
{
	if(pHead != NULL)
	{
		while(proc_block->next != NULL)
		{
			proc_block = proc_block->next;
		}
	}
	
	int i;
	for(i=0; i<num; i++)
	{
		pBlock *process;
		process = (pBlock*)malloc(sizeof(pBlock));
		
		printf("Enter size of the Process Running %d: ",count_pBlock);
		scanf("%d",&process->size);
		
		while(mem_block != NULL)
		{
			if(mem_block->size >= process->size)
			{
				break;
			}
			else
			{
				mem_block = mem_block->next;
			}
		}
		
		if(mem_block == NULL)
		{
			printf("\nNot enough space!!\n");
			i--;
			mem_block = mHead;
			free(process);
			continue;
		}
		
		if(pHead == NULL)
		{
			process->next = NULL;
			process->prev = NULL;
			pHead = process;
		}
		else
		{
			process->next = NULL;
			process->prev= proc_block;
			
			if(pHead->next == NULL)
			{
				pHead->next = process;
			}
			else
			{
				proc_block->next = process;
			}
		}
		
		process->serial_num = count_pBlock;
		mem_block->size = mem_block->size - process->size;
		process->block_num = mem_block->serial_num;
		proc_block = process;
		mem_block = mHead;
		count_pBlock++;
	}
	
}

void display_pBlocks(pBlock *proc_block)
{
	while(proc_block != NULL)
	{
		printf("| %d [%d] |  ", proc_block->serial_num, proc_block->size);
		proc_block = proc_block->next;
	}
	printf("\n");
}

void display_mBlocks(mBlock *mem_block)
{
	while(mem_block != NULL)
	{
		printf("| %d [%d] |  ",mem_block->serial_num, mem_block->size);
		mem_block = mem_block->next;
	}
	printf("\n");
}

void rProcess(pBlock *proc_block)
{
	int i=1;
	while(proc_block != NULL)
	{
		proc_block->serial_num = i;
		proc_block = proc_block->next;
		i++;
	}
	display_pBlocks(pHead);
}

void pTerminate (mBlock *mem_block, pBlock *proc_block, int num)
{
	int i;
	for(i=0; i<num; i++)
	{
		int terminate;
		
		printf("Enter the process number you want to terminate  ");
		scanf("%d",&terminate);
		
		while(proc_block != NULL && proc_block->serial_num != terminate)
		{
			proc_block = proc_block->next;
		}
	
		while(mem_block != NULL && proc_block->serial_num != proc_block->block_num)
		{
			mem_block = mem_block->next;
		}
		mem_block->size = mem_block->size + proc_block->size;
		
		while(proc_block)
		{
			if (proc_block->next == NULL && mem_block->prev==NULL)
			{
				pHead = NULL;
				free(proc_block);
				count_pBlock--;
				break;
			}

			if(proc_block->next != NULL && proc_block->prev==NULL)
			{
				proc_block->next->prev=NULL;
				pHead = proc_block->next;
				free(proc_block);
				count_pBlock--;
				break;
			}
			else
			{
				proc_block->prev->next = proc_block->next;
				free(proc_block);
				count_pBlock--;
				break;
			}
		}

		rProcess(pHead);
		printf("\n");
		mem_block = mHead;
		proc_block = pHead;
	}
}

int main()
{

	printf("Heap Implementation Using First Fit approach\n\n");
	printf("What operations u want to perform \n\n");
	
	int choice;
	do{
		int n;
		
		printf("1: Create memory blocks\n");
		printf("2: Allocate processes using First Fit\n");
		printf("3: Free a process\n");
		printf("4: Exit!!\n");
		
		printf("Enter the action number : ");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:
				{
					printf("Enter number of memory blocks you want to add : ");
					scanf("%d",&n);
					create_mBlock(mHead,n);
					printf("\nMemory Blocks: ");
					display_mBlocks(mHead);
					break;
				}
				
			case 2:
				{
					printf("Enter the number of processes you want to start : ");
					scanf("%d",&n);
					startProcess(mHead, pHead, n);
					printf("\nMemory Blocks: ");
					display_mBlocks(mHead);
					printf("\nProcess Blocks: ");
					display_pBlocks(pHead);
					break;
				}

			case 3:
				{
					printf("Enter the number of processes you want to terminate : ");
					scanf("%d",&n);
					pTerminate(mHead, pHead, n);
					printf("\nMemory Blocks: ");
					display_mBlocks(mHead);
					printf("\nProcess Blocks:  ");
					display_pBlocks(pHead);
					break;
				}

			case 4:
				{
					printf("\n\n\n END!!");
					break;
				}

			default:
				{
					printf("\nNot Valid Number!!");
					break;
				}
		}
		printf("\n");

	}while(choice!=4);
}		

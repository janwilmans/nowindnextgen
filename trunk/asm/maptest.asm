	macro PRINT addr
	
	ld de, addr
	ld c,9
	call 5
	endmacro
	
    macro DEBUGMESSAGE string
        ld d,d
	jr .skip
        db string
.skip:
    endmacro
        	
  output maptest.com

  defpage 0,100h

  page 0

  code @ 100h
entry

  DEBUGMESSAGE "Mapper Test v2"
  
  ; assume we execute from page 0, bank 3 and dont change out($fc)!
  ld a,4

  out ($fd),a
  ld hl,$aa55
  ld ($4000),hl

  out ($fe),a
  ld de,($8000)
  
  and a		; reset carry
  sbc hl,de 
  jr nz,fault1  ; page 2/3 fault
  DEBUGMESSAGE "PAGE 2/3 read/write OK"
  jr exit
fault1:  
  DEBUGMESSAGE "PAGE 2/3 read/write FAILED"
  
  
  
  
exit  
  ; test if a CALL works (stack ok ?)
  PRINT welcome
  PRINT done
loop
  jp loop
  
  
  
  
  
  
  
  code
welcome
  byte "Welcome stack!\n$"
done
  byte "Mapper test done.\n$"

  end



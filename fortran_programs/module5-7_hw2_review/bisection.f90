! bisection.f90 program 

program main
  use params

  implicit none

  interface
    subroutine bisect_hist(hist, a, b, tol, it)
    real, dimension(:), intent(inout) :: hist
    real, intent(inout) :: a, b
    real, intent(in)    :: tol
    integer,  intent(out)   :: it

    end subroutine bisect_hist
  end interface

  real :: tol
  integer  :: niter1, niter2
  real :: r1, r2
  real, external :: f
  INTEGER :: i

  real :: hist1(n), hist2(n)

  tol = 1.0e-4

  call bisect_hist(hist1, a1, b1, tol, niter1)
  if (niter1 >= 1) then
    write(*,'(A,1X,F12.7,1X,A,1X,ES12.4)') "Root on [-4,-2]:", hist1(niter1-1), "f(x*)=", f(hist1(niter1-1))
    write(*,'(A,1X,I0)') "Iterations:", niter1
    OPEN(1,file='output.txt')
    do i = 1, niter1
      WRITE(1,102) hist1(i)
    end do
  else
    write(*,*) "Error: No sign change or failure on [-4,-2]."
  end if

  CLOSE(1)
  CALL flush(1)

  call bisect_hist(hist2, a2, b2, tol, niter2)
  if (niter2 >= 1) then
    write(*,'(A,1X,F12.7,1X,A,1X,ES12.4)') "Root on [2,4]:", hist1(niter2-1), "f(x*)=", f(hist2(niter2-1))
    write(*,'(A,1X,I0)') "Iterations:", niter2
  else
    write(*,*) "Error: No sign change or failure on [2,4]."
  end if
 
 102 FORMAT(F0.17) ! Floating point format specifier, with 17 decimal places; the 0 here means that processor selects the smallest positive field width necessary
end program main


! ---------- external subroutine ----------

subroutine bisect_hist(hist, a, b, tol, it)
    implicit none
    real, dimension(:), intent(inout) :: hist
    real, intent(inout) :: a, b
    real, intent(in)    :: tol
    integer,  intent(out)   :: it

    real :: mid

    it = 0

    write(*,'(A," [a,b] = [",F0.6,", ",F0.6,"]")') "Execution for interval:", a, b

    if ( f(a) * f(b) >= 0.0 ) then
        write(*,'(A," [a,b] = [",F0.6,", ",F0.6,"]")') &
        "f(a)f(b)<0 not satisfied! f has no root in", a, b
        it = -1
        return
    end if

    do while ( abs(b - a) > tol )
        mid = 0.5 * (a + b)
        it = it + 1
        hist(it) = mid

        if ( f(a) * f(mid) < 0.0 ) then
            b = mid
        else
            a = mid
        end if

    end do
end subroutine bisect_hist


! ---------- external function ----------
real function f(x)
  implicit none
  real, intent(in) :: x
  f = exp(-x) * ( 3.2 * sin(x) - 0.5 * cos(x) ) - 3.0
end function f